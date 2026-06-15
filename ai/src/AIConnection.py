import socket
import sys


class AIConnection:
    def __init__(self, host, port, team_name):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.socket = None
        self.connect(host, port)
        self.do_handshake()

    def connect(self, host, port):
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error:
            print("Failed to create socket.")
            sys.exit(84)

        try:
            self.socket.connect((host, port))
        except socket.error:
            print(f"Failed to connect at port {port}.")
            sys.exit(84)

    def do_handshake(self):
        welcome_msg = self.socket.recv(1024).decode()
        if welcome_msg != "WELCOME\n":
            print("Handshake failed at welcome message")
            sys.exit(84)
        self.socket.send((self.team_name + "\n").encode())
        client_num = self.socket.recv(1024).decode().strip()
        dims = self.socket.recv(1024).decode().strip()
        if client_num.isdigit() and int(client_num) > 0:
            pass
        else:
            print("Handshake failed, no space for a new client or team name incorrect")
            sys.exit(84)
        if dims == "":
            print("Handshake failed, no dimensions given")
            sys.exit(84)

    def send_command(self, cmd):
        self.socket.send((cmd + "\n").encode())
        return self.read_line()

    def read_line(self):
        buffer = ""
        while True:
            char = self.socket.recv(1).decode()
            if not char:
                break
            buffer += char
            if char == "\n":
                break
        return buffer.strip()

    def disconnect(self):
        if self.socket:
            self.socket.close()
