import socket
import sys
import threading
import time
from queue import Queue
from collections import deque


class CommandRequest:
    def __init__(self, command, id, time_answer):
        self.command = command
        self.id = id

    def get_id(self):
        return self.id

    def get_command(self):
        return self.command


class CommandResponse:
    def __init__(self, command, id, time_answered):
        self.command = command
        self.id = id
        self.time_answered = time_answered


class Connection:
    MAX_PENDING_COMMANDS = 10

    def __init__(self, host, port, team_name):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.socket = None
        self.running = True
        self.check_response = True
        self.sub_character = "\n"
        self.response_queue = Queue()
        self.socket_lock = threading.Lock()
        self.command_lock = threading.Lock()
        self.response_lock = threading.Lock()
        self.broadcast_lock = threading.Lock()
        self.reader_thread = None
        self.event_thread = None
        self.connect(host, port)
        self.do_handshake()
        self.start_reader()
        self.start_receive_poll_cmd()
        self.pending_commands: Dict[int, CommandRequest] = {}
        self.command_queue = deque()
        self.event_queue = deque()
        self.run_receive_poll_cmd()
        self.response_buffer: Dict[int, CommandRequest] = {}
        self.failed_commands = deque()
        self.cmd_counter = 0

    def connect(self, host, port):
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error as e:
            print(f"Failed to create socket: {e}")
            sys.exit(84)
        try:
            self.socket.connect((host, port))
        except socket.error as e:
            print(f"Failed to connect to {host}:{port}: {e}")
            sys.exit(84)

    def do_handshake(self):
        try:
            welcome_msg = self.socket.recv(1024).decode()
            if welcome_msg != "WELCOME\n":
                print(f"Handshake failed: expected 'WELCOME\\n', got '{welcome_msg}'")
                sys.exit(84)

            with self.socket_lock:
                self.socket.send((self.team_name + self.sub_character).encode())
            client_num = self.socket.recv(1024).decode().strip()
            if not client_num.isdigit() or int(client_num) <= 0:
                print(f"Handshake failed: invalid client number '{client_num}'")
                sys.exit(84)
            self.client_num = int(client_num)
            dims = self.socket.recv(1024).decode().strip()
            if not dims:
                print("Handshake failed: no dimensions received")
                sys.exit(84)
            try:
                x, y = map(int, dims.split())
                self.width = x
                self.height = y
            except ValueError:
                print(f"Handshake failed: invalid dimensions '{dims}'")
                sys.exit(84)
        except Exception as e:
            print(f"Handshake error: {e}")
            sys.exit(84)

    def start_reader(self):
        self.reader_thread = threading.Thread(
            target=self._run_reader, daemon=False, name="Connection-Reader"
        )
        self.reader_thread.start()

    def _run_reader(self):
        buffer = ""
        try:
            self.socket.setblocking(False)
            while self.running:
                try:
                    data = self.socket.recv(1024).decode()
                    if not data:
                        print("Server closed connection")
                        self.running = False
                        break
                    buffer += data
                    while self.sub_character in buffer:
                        line, buffer = buffer.split(self.sub_character, 1)
                        line = line.strip()
                        if not line:
                            continue
                        msg_type, msg_data = self._parse_server_message(line)
                        if msg_type == "broadcast":
                            with self.broadcast_lock:
                                self.broadcast_queue.append(
                                    BroadcastMessage(
                                        direction=msg_data["direction"],
                                        message=msg_data["message"],
                                    )
                                )
                        elif msg_type == "eject":
                            with self.event_lock:
                                self.event_queue.append(
                                    ServerEvent(event_type="eject", data=msg_data)
                                )
                            if self.on_event_received:
                                self.on_event_received("eject", msg_data)
                        elif msg_type == "dead":
                            with self.event_lock:
                                self.event_queue.append(
                                    ServerEvent(event_type="dead", data=msg_data)
                                )
                        elif msg_type == "elevation":
                            with self.event_lock:
                                self.event_queue.append(
                                    ServerEvent(event_type="elevation", data=msg_data)
                                )
                        else:
                            with self.response_lock:
                                self.command_responses.append(msg_data["content"])
                except BlockingIOError:
                    time.sleep(0.01)
                except Exception as e:
                    if self.running:
                        print(f"Error reading from socket: {e}")
                    self.running = False
                    break
        except Exception as e:
            print(f"Fatal reader socket error: {e}")
            self.running = False

    def send_command(self, cmd: str):
        if not self.running:
            print("Cannot send: connection is not active")
            return 84
        with self.command_lock:
            self.cmd_counter += 1
            cmd_id = self.cmd_counter
            cmd_request = CommandRequest(
                command=cmd,
                id=cmd_id,
            )
            self.command_queue.append(cmd_request)
        return cmd_id

    def disconnect(self):
        self.running = False
        try:
            if self.socket:
                self.socket.close()
        except Exception as e:
            print(f"Error closing socket: {e}")
        if self.reader_thread and self.reader_thread.is_alive():
            self.reader_thread.join(timeout=2.0)
            if self.reader_thread.is_alive():
                print("Warning: reader thread did not terminate gracefully")

    def parse_server_message(self, line):
        line = line.strip()
        if not line:
            return None, None
        match = re.match(r"message\s+(\d+),\s*(.*)", line)
        if match:
            direction = int(match.group(1))
            message = match.group(2)
            return "broadcast", {"direction": direction, "message": message}
        if line.startswith("eject:"):
            match = re.match(r"eject:\s*(\d+)", line)
            if match:
                direction = int(match.group(1))
                return "eject", {"direction": direction}
        if line == "dead":
            return "dead", {}
        if "Elevation" in line or "Current level" in line:
            return "elevation", {"message": line}
        return "response", {"content": line}

    def send_cmd_buffer(self):
        with self.command_lock:
            while (
                len(self.pending_commands) < self.MAX_PENDING_COMMANDS
                and len(self.command_queue) > 0
            ):
                cmd_request = self.command_queue.popleft()
                if self._send_raw_command(cmd_request.command):
                    cmd_request.sent_at = time.time()
                    self.pending_commands[cmd_request.cmd_id] = cmd_request
                else:
                    self.command_queue.appendleft(cmd_request)
                    time.sleep(0.1)
                    break

    def run_receive_poll_cmd(self):
        while self.running:
            try:
                self.check_response()
                self.send_cmd_buffer()
                time.sleep(0.05)
            except Exception as e:
                print(f"Error in command worker: {e}")

    def start_receive_poll_cmd(self):
        self.event_thread = threading.Thread(
            target=self.run_receive_poll_cmd, daemon=False, name="Connection-Receive"
        )
        self.reader_thread.start()

    def check_response(self):
        while self.command_responses:
            try:
                response_str = self.command_responses.popleft()

                with self.command_lock:
                    if not self.pending_commands:
                        print(
                            f"Unexpected response (no pending commands/..): {response_str}"
                        )
                        continue
                    first_cmd_id = next(iter(self.pending_commands.keys()))
                    cmd = self.pending_commands.pop(first_cmd_id)
                    cmd_response = CommandResponse(
                        cmd_id=first_cmd_id, response=response_str
                    )
                    with self.response_lock:
                        self.response_buffer[first_cmd_id] = cmd_response
            except IndexError:
                break
            except Exception as e:
                print(f"Error processing command response: {e}")

    def get_broadcasts(self):
        with self.broadcast_lock:
            broadcasts = list(self.broadcast_queue)
            self.broadcast_queue.clear()
            return broadcasts

    def get_next_broadcast(self):
        with self.broadcast_lock:
            if self.broadcast_queue:
                return self.broadcast_queue.popleft()
            return None

    def broadcast_count(self):
        with self.broadcast_lock:
            return len(self.broadcast_queue)

    def get_events(self):
        with self.event_lock:
            events = list(self.event_queue)
            self.event_queue.clear()
            return events

    def get_next_event(self):
        with self.event_lock:
            if self.event_queue:
                return self.event_queue.popleft()
            return None

    def event_count(self):
        with self.event_lock:
            return len(self.event_queue)

    def __del__(self):
        try:
            self.disconnect()
        except socket.error as e:
            print(f"Error while disconnecting '{e}'")
