import socket
import subprocess
import time
import pytest
import yaml

# Load Config
with open("config.yaml", "r") as f:
    CONFIG = yaml.safe_load(f)

class ZappyClient:
    def __init__(self, port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect(("127.0.0.1", port))
        self.buffer = ""

    def send(self, msg: str):
        self.sock.sendall(msg.encode("utf-8"))

    def read_until(self, delimiter="\n", timeout=2.0) -> str:
        self.sock.settimeout(timeout)
        while delimiter not in self.buffer:
            try:
                chunk = self.sock.recv(4096).decode("utf-8")
                if not chunk:
                    raise ConnectionError("Server closed connection.")
                self.buffer += chunk
            except socket.timeout:
                raise TimeoutError(f"Timeout waiting for '{delimiter}'. Buffer: {self.buffer}")
        
        parts = self.buffer.split(delimiter, 1)
        line = parts[0] + delimiter
        self.buffer = parts[1]
        return line
        
    def close(self):
        self.sock.close()

def get_free_port():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(("", 0))
        return s.getsockname()[1]

@pytest.fixture(params=["reference", "custom"])
def server(request):
    binary_path = CONFIG["binaries"][request.param]
    port = get_free_port()
    args = CONFIG["server_args"]
    
    cmd = [
        binary_path, 
        "-p", str(port), 
        "-x", args["width"], "-y", args["height"], 
        "-c", args["clients_per_team"], 
        "-f", args["freq"],
        "-n"
    ] + args["teams"]
    
    process = subprocess.Popen(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.1)
    
    yield port, request.param
    
    process.terminate()
    process.wait()
