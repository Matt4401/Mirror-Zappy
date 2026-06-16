import sys
import os
import unittest
from unittest.mock import patch, MagicMock
import socket
import threading
import time

# Adjust this path to your project structure
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from AIConnection import AIConnection

class TestAIConnectionUnit(unittest.TestCase):
    @patch("AIConnection.socket.socket")
    def test_successful_handshake(self, mock_socket_class):
        mock_socket = MagicMock()
        mock_socket_class.return_value = mock_socket

        # Simulate the handshake sequence
        mock_socket.recv.side_effect = [b"WELCOME\n", b"2\n", b"10 10\n"]

        ai = AIConnection("127.0.0.1", 4242, "TeamA")

        mock_socket.connect.assert_called_with(("127.0.0.1", 4242))
        mock_socket.send.assert_called_with(b"TeamA\n")
        self.assertEqual(ai.client_num, 2)
        self.assertEqual(ai.width, 10)

    @patch("AIConnection.socket.socket")
    def test_handshake_bad_welcome(self, mock_socket_class):
        mock_socket = MagicMock()
        mock_socket_class.return_value = mock_socket
        mock_socket.recv.return_value = b"BAD MESSAGE\n"

        with self.assertRaises(SystemExit) as cm:
            AIConnection("127.0.0.1", 4242, "TeamA")
        self.assertEqual(cm.exception.code, 84)

class TestAIConnectionFunctional(unittest.TestCase):
    def setUp(self):
        self.host = "127.0.0.1"
        self.port = 8889 # Use a different port to avoid conflicts
        self.server_ready = threading.Event()
        self.server_thread = threading.Thread(target=self._run_dummy_server, daemon=True)
        self.server_thread.start()
        self.server_ready.wait(timeout=2.0)

    def _run_dummy_server(self):
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server.bind((self.host, self.port))
        server.listen(1)
        self.server_ready.set()
        conn, addr = server.accept()
        conn.sendall(b"WELCOME\n")
        conn.recv(1024) # Consume team name
        conn.sendall(b"1\n")
        conn.sendall(b"10 10\n")

        # Keep connection open for commands
        data = conn.recv(1024)
        if data == b"Forward\n":
            conn.sendall(b"ok\n")
        conn.close()
        server.close()

    def test_full_connection_and_command(self):
        ai = AIConnection(self.host, self.port, "MyTeam")
        success = ai.send_command("Forward")
        self.assertTrue(success)

        # Verify response in queue (needs small delay for thread)
        time.sleep(0.1)
        response = ai.response_queue.get(timeout=1.0)
        self.assertEqual(response, "ok")
        ai.disconnect()

if __name__ == "__main__":
    unittest.main()