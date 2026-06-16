import socket
import sys
import threading
import time
from queue import Queue, Empty


class AIConnection:
    def __init__(self, host, port, team_name, debug=False):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.socket = None
        self.running = True
        self.sub_charac = "\n"
        self.response_queue = Queue()
        self.socket_lock = threading.Lock()
        self.reader_thread = None
        self._connect(host, port)
        self._do_handshake()
        self._start_reader()

    def _connect(self, host, port):
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

    def _do_handshake(self):
        try:
            welcome_msg = self.socket.recv(1024).decode()
            if welcome_msg != "WELCOME\n":
                print(f"Handshake failed: expected 'WELCOME\\n', got '{welcome_msg}'")
                sys.exit(84)

            with self.socket_lock:
                self.socket.send((self.team_name + self.sub_charac).encode())
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

    def _start_reader(self):
        self.reader_thread = threading.Thread(
            target=self._run_reader,
            daemon=False,
            name="AIConnection-Reader"
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
                        self.running = False
                        break
                    buffer += data
                    while self.sub_charac in buffer:
                        line, buffer = buffer.split(self.sub_charac, 1)
                        line = line.strip()
                        if line:
                            self.response_queue.put(line)
                except BlockingIOError:
                    time.sleep(0.01)
                    continue
                except Exception as e:
                    if self.running:
                        print(f"Error reading from socket: {e}")
                    self.running = False
                    break
        except Exception as e:
            print(f"Fatal reader socket error: {e}")
            self.running = False


    def send_command(self, cmd):
        if not self.running or not self.socket:
            print("Cannot send: connection not active")
            return False
        try:
            with self.socket_lock:
                self.socket.send((cmd + self.sub_charac).encode())
            return True
        except Exception as e:
            print(f"Error sending command '{cmd}': {e}")
            self.running = False
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
                print(f"Warning: reader thread did not terminate gracefully")


    def __del__(self):
        try:
            self.disconnect()
        except:
            print(f"Error while disconnecting happendd")