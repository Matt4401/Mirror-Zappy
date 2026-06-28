import unittest
from unittest.mock import patch, MagicMock
from src.Connection import Connection

class TestConnection(unittest.TestCase):
    @patch("socket.socket")
    def setUp(self, mock_socket_class):
        self.mock_socket_instance = MagicMock()
        mock_socket_class.return_value = self.mock_socket_instance
        self.mock_socket_instance.recv.return_value = b"WELCOME\n1\n10 10\n"
        self.conn = Connection("localhost", 1234, "TeamA")

    def test_initialization_values(self):
        self.assertEqual(self.conn.host, "localhost")
        self.assertEqual(self.conn.port, 1234)
        self.assertEqual(self.conn.team_name, "TeamA")
        self.assertTrue(self.conn.running)

    def test_send_command(self):
        cmd_id = self.conn.send_command("Forward")
        self.assertEqual(cmd_id, 1)
        self.assertEqual(len(self.conn.command_queue), 1)

    def test_send_raw_command(self):
        # Reset the mock to clear the 'TeamA\n' call from the setUp handshake
        self.conn.socket.sendall.reset_mock()

        # Now perform the action we actually want to test
        result = self.conn.send_raw_command("Test")

        # Assertions
        self.assertTrue(result)
        self.conn.socket.sendall.assert_called_once_with(b"Test\n")

    def test_parse_server_message_broadcast(self):
        msg_type, data = self.conn.parse_server_message("message 3, text")
        self.assertEqual(msg_type, "broadcast")
        self.assertEqual(data["direction"], 3)
        self.assertEqual(data["message"], "text")

    def test_parse_server_message_eject(self):
        msg_type, data = self.conn.parse_server_message("eject: 5")
        self.assertEqual(msg_type, "eject")
        self.assertEqual(data["direction"], 5)

    def test_parse_server_message_elevation(self):
        msg_type, data = self.conn.parse_server_message("Current level: 2")
        self.assertEqual(msg_type, "elevation")
        self.assertEqual(data["message"], "Current level: 2")

    def tearDown(self):
        self.conn.disconnect()