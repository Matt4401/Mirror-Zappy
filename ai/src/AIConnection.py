import socket
import sys


class AIConnection:
    def __init__(self, host, port, team_name, data_lock, list):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.socket = None
        self.connect(host, port)
        self.do_handshake()
        self.runnig = true
        self.sub_charac = "\n"

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
        self.socket.send((self.team_name + self.sub_charac).encode())
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
        self.socket.send((cmd + self.sub_charac).encode())
        return self.read_line()

    def add_in_list(self, buffer, nbr_backslash_n):
        answer = ""
        for i in nbr_backslash_n:
            answer = buffer.split(self.sub_charac)[i]
            with data_lock:
                self.answer_list.append(answer)
        buffer = buffer.remove_prefix(answer)
        return buffer


    def read_line(self):
        buffer = ""
        while True:
            buffer = self.socket.recv(1024).decode()
            nbr_backslash_n = buffer.count(self.sub_charac)
            if nbr_backslash_n > 0:
                buffer = add_in_list(buffer, nbr_backslash_n)

    def disconnect(self):
        if self.socket:
            self.socket.close()

def run_reader(self):
    buffer = ""
    while self.running:
        try:
            data = self.socket.recv(1024).decode()
            if not data: break
            buffer += data

            while self.sub_charac in buffer:
                line, buffer = buffer.split(self.sub_charac, 1)
                with self.data_lock:
                    if line.strip():
                        self.answer_list.append(line.strip())
        except Exception as e:
            print(f"Error while reading the socket content: {e}")
            break