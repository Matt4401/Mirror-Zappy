import sys
import os
import unittest
from unittest.mock import patch, MagicMock
import socket
import threading
import time

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "../../..")))

from ai.src.AIConnection import AIConnection


class TestAIConnectionUnit(unittest.TestCase):
    @patch("ai.src.AIConnection.socket.socket")
    def test_successful_handshake(self, mock_socket_class):
        mock_socket = MagicMock()
        mock_socket_class.return_value = mock_socket

        mock_socket.recv.side_effect = [b"WELCOME\n", b"2\n", b"10 10\n"]

        ai = AIConnection("127.0.0.1", 4242, "TeamA")

        mock_socket.connect.assert_called_with(("127.0.0.1", 4242))
        mock_socket.send.assert_called_with(b"TeamA\n")
        self.assertEqual(ai.team_name, "TeamA")

    @patch("ai.src.AIConnection.socket.socket")
    def test_handshake_bad_welcome(self, mock_socket_class):
        mock_socket = MagicMock()
        mock_socket_class.return_value = mock_socket

        mock_socket.recv.return_value = b"BAD MESSAGE aaaaaaaaaa\n"

        with self.assertRaises(SystemExit) as cm:
            AIConnection("127.0.0.1", 4242, "TeamA")

        self.assertEqual(cm.exception.code, 84)


class TestAIConnectionFunctional(unittest.TestCase):
    def setUp(self):
        self.host = "127.0.0.1"
        self.port = 8888
        self.server_ready = threading.Event()

        self.server_thread = threading.Thread(target=self._run_dummy_server)
        self.server_thread.daemon = True
        self.server_thread.start()
        self.server_ready.wait(timeout=2.0)

    def _run_dummy_server(self):
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            server.bind((self.host, self.port))
            server.listen(1)
        except Exception:
            self.server_ready.set()
            return
        self.server_ready.set()
        try:
            conn, addr = server.accept()
            conn.sendall(b"WELCOME\n")
            team_name = conn.recv(1024).decode()
            if team_name == "MyTeam\n":
                conn.sendall(b"1\n")
                time.sleep(0.05)
                conn.sendall(b"10 10\n")
            cmd = conn.recv(1024).decode()
            if cmd == "Forward\n":
                conn.sendall(b"ok\n")
            conn.close()
        except Exception:
            pass
        finally:
            server.close()

    def test_full_connection_and_command(self):
        ai = AIConnection(self.host, self.port, "MyTeam")
        response = ai.send_command("Forward")
        self.assertEqual(response, "ok")
        ai.disconnect()


if __name__ == "__main__":
    unittest.main()
