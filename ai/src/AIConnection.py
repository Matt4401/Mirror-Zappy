import socket

class AIConnection:
    def __init__(self, host, port, team_name):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.connect(host, port)
        self.do_handshake()

    def connect(self, host, port):
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error:
            print("Failed to create socket.")
        try:
            s.connect((host, port))
        except socket.error:
            print("Failed to connect at port .")

    def do_handshake(self):
        welcome_msg = self.socket.recv(1024).decode().strip()
        self.socket.send((self.team_name + "\n").encode())
        client_num = self.socket.recv(1024).decode().strip()
        dims = self.socket.recv(1024).decode().strip()
        if welcome_msg != "WELCOME\n":
            print("Handshake failed at welcome message")
            exit(84)
        if int(client_num) < 1:
            print("Handshake failed, no space for a new client")
            exit(84)
        if dims == "":
            print("Handshake failed, no dimensions given")
            exit(84)


    def send_command(self, cmd):
        self.socket.send((cmd + "\n").encode())
        return self._read_line()

    def disconnect(self, socket):
        socket.close()

