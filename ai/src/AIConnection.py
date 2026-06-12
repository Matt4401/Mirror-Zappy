import socket

class AIConnection:
    def __init__(self, name):
        self.name = name
    def connect(self):
        port = 80
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error:
            print("Failed to create socket.")
        try:
            s.connect(('127.0.0.1', port))
        except socket.error:
            print("Failed to connect at port .")

    def do_handshake(self):
        pass

    def disconnect(self, socket):
        socket.close()

