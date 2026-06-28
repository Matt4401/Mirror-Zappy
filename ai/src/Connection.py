import socket
import sys
import threading
import time
import re
from typing import Dict
from collections import deque


class BroadcastMessage:
    def __init__(self, direction, message):
        self.direction = direction
        self.message = message


class ServerEvent:
    def __init__(self, event_type, message):
        self.event_type = event_type
        self.data = message


class CommandRequest:
    def __init__(self, command, id):
        self.command = command
        self.id = id

    def get_id(self):
        return self.id

    def get_command(self):
        return self.command


class CommandResponse:
    def __init__(self, command, id, success):
        self.command = command
        self.id = id
        self.success = success


class Connection:
    MAX_PENDING_COMMANDS = 10

    def __init__(self, host, port, team_name, connect_timeout=0.0):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.socket = None
        self.running = False
        self.message_delimiter = "\n"
        self.command_queue = deque()
        self.active_requests: Dict[int, CommandRequest] = {}
        self.broadcast_queue = deque()
        self.event_queue = deque()
        self.command_responses = deque()
        self.response_buffer: Dict[int, CommandResponse] = {}
        self.response_events: Dict[int, threading.Event] = {}
        self.socket_lock = threading.Lock()
        self.command_lock = threading.Lock()
        self.response_lock = threading.Lock()
        self.broadcast_lock = threading.Lock()
        self.event_lock = threading.Lock()
        self.next_command_id = 0
        self.reader_thread = None
        self.on_event_received = None
        self.event_thread = None
        deadline = time.time() + connect_timeout
        while True:
            self.connect(host, port)
            if self.do_handshake():
                break
            try:
                self.socket.close()
            except OSError:
                pass
            if time.time() >= deadline:
                print("No team slot available")
                sys.exit(84)
            time.sleep(0.3)
        self.start()

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
        buffer = ""
        idx = 0
        self.socket.setblocking(False)
        while idx < 3:
            try:
                data = self.socket.recv(1024).decode()
                if not data:
                    print("Handshake failed: Connection closed by server")
                    sys.exit(84)
                buffer += data
            except (BlockingIOError, InterruptedError):
                time.sleep(0.01)
                continue
            except Exception as e:
                print(f"Handshake error: {e}")
                sys.exit(84)
            while self.message_delimiter in buffer and idx < 3:
                line, buffer = buffer.split(self.message_delimiter, 1)
                line = line.strip()
                if not line:
                    continue
                if idx == 0:
                    if line != "WELCOME":
                        print(f"Handshake failed: expected 'WELCOME', got '{line}'")
                        sys.exit(84)
                    idx += 1
                    self.send_raw_command(self.team_name)
                elif idx == 1:
                    if line == "ko":
                        return False
                    if not line.isdigit():
                        print(f"Handshake failed: invalid client number '{line}'")
                        sys.exit(84)
                    self.client_num = int(line)
                    idx += 1
                elif idx == 2:
                    try:
                        x, y = map(int, line.split())
                        self.width = x
                        self.height = y
                        idx += 1
                    except ValueError:
                        print(f"Handshake failled: invalid dimensions '{line}'")
                        sys.exit(84)
        return True

    def start(self):
        self.running = True
        self.reader_thread = threading.Thread(
            target=self.run_reader, daemon=False, name="Connection-Reader"
        )
        self.reader_thread.start()
        self.event_thread = threading.Thread(
            target=self.run_receive_poll_cmd, daemon=False, name="Connection-Receive"
        )
        self.event_thread.start()

    def run_reader(self):
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
                    while self.message_delimiter in buffer:
                        line, buffer = buffer.split(self.message_delimiter, 1)
                        line = line.strip()
                        if not line:
                            continue
                        msg_type, msg_data = self.parse_server_message(line)
                        if msg_type == "pending":
                            continue
                        if msg_type == "dead":
                            self.running = False
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
                                    ServerEvent(event_type="eject", message=msg_data)
                                )
                            if self.on_event_received:
                                self.on_event_received("eject", msg_data)
                        elif msg_type == "dead":
                            with self.event_lock:
                                self.event_queue.append(
                                    ServerEvent(event_type="dead", message=msg_data)
                                )
                        elif msg_type == "elevation":
                            with self.event_lock:
                                self.event_queue.append(
                                    ServerEvent(
                                        event_type="elevation", message=msg_data
                                    )
                                )
                            if msg_data.get("message", "").startswith("Current level"):
                                with self.command_lock:
                                    has_incantation = any(
                                        req.command == "Incantation"
                                        for req in self.active_requests.values()
                                    )
                                if has_incantation:
                                    with self.response_lock:
                                        self.command_responses.append(msg_data["message"])
                        else:
                            with self.response_lock:
                                self.command_responses.append(msg_data["content"])
                except BlockingIOError:
                    time.sleep(0.01)
                    # pass
                except Exception as e:
                    if self.running:
                        print(f"Error reading from socket: {e}")
                    self.running = False
                    break
        except Exception as e:
            print(f"Fatal reader socket error: {e}")
            self.running = False

    def send_command(self, cmd):
        if not self.running:
            return 84
        with self.command_lock:
            self.next_command_id += 1
            cmd_id = self.next_command_id
            cmd_request = CommandRequest(
                command=cmd,
                id=cmd_id,
            )
            self.command_queue.append(cmd_request)
        return cmd_id

    def send_raw_command(self, cmd):
        try:
            with self.socket_lock:
                self.socket.sendall((cmd + self.message_delimiter).encode())
            return True
        except Exception as e:
            print(f"Failed to send command: {e}")
            return False

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
        if line == "Elevation underway":
            return "pending", {"content": line}
        if line.startswith("Current level"):
            return "elevation", {"message": line}

        return "response", {"content": line}

    def send_cmd_buffer(self):
        with self.command_lock:
            while (
                len(self.active_requests) < self.MAX_PENDING_COMMANDS
                and len(self.command_queue) > 0
            ):
                cmd_request = self.command_queue.popleft()
                if self.send_raw_command(cmd_request.command):
                    cmd_request.sent_at = time.time()
                    self.active_requests[cmd_request.id] = cmd_request
                else:
                    self.command_queue.appendleft(cmd_request)
                    time.sleep(0.1)
                    break

    def run_receive_poll_cmd(self):
        while self.running:
            try:
                self.process_incoming_responses()
                self.send_cmd_buffer()
                time.sleep(0.05)
            except Exception as e:
                print(f"Error in command worker: {e}")
                time.sleep(0.05)
        self.process_incoming_responses()

    def process_incoming_responses(self):
        while True:
            try:
                response_str = self.command_responses.popleft()
                with self.command_lock:
                    if not self.active_requests:
                        print(
                            f"Unexpected response (no pending commands/..): {response_str}"
                        )
                        continue
                    first_cmd_id = next(iter(self.active_requests.keys()))
                    del self.active_requests[first_cmd_id]
                    success = response_str.lower() != "ko"
                    cmd_response = CommandResponse(
                        command=response_str, id=first_cmd_id, success=success
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

    def get_command_response(self, cmd_id, timeout=5.0):
        start_time = time.time()
        while time.time() - start_time < timeout:
            with self.response_lock:
                if cmd_id in self.response_buffer:
                    cmd_response = self.response_buffer.pop(cmd_id)
                    with self.command_lock:
                        if cmd_id in self.active_requests:
                            del self.active_requests[cmd_id]
                    return cmd_response.success, cmd_response.command
            time.sleep(0.005)
        return None
