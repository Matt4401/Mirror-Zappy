import sys
import os
import unittest
from unittest.mock import patch, MagicMock
import socket
import threading
import time

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from Connection import Connection, BroadcastMessage, ServerEvent, CommandRequest


class TestConnectionUnit(unittest.TestCase):
    @patch("Connection.socket.socket")
    def test_successful_handshake(self, mock_socket_class):
        mock_socket = MagicMock()
        mock_socket_class.return_value = mock_socket
        mock_socket.recv.side_effect = [b"WELCOME\n", b"2\n", b"10 10\n"]

        ai = Connection("127.0.0.1", 4242, "TeamA")

        mock_socket.connect.assert_called_with(("127.0.0.1", 4242))
        mock_socket.send.assert_called_with(b"TeamA\n")
        self.assertEqual(ai.client_num, 2)
        self.assertEqual(ai.width, 10)
        self.assertEqual(ai.height, 10)

    @patch("Connection.socket.socket")
    def test_handshake_bad_welcome(self, mock_socket_class):
        mock_socket = MagicMock()
        mock_socket_class.return_value = mock_socket
        mock_socket.recv.return_value = b"BAD MESSAGE\n"

        with self.assertRaises(SystemExit) as cm:
            Connection("127.0.0.1", 4242, "TeamA")
        self.assertEqual(cm.exception.code, 84)

    @patch("Connection.Connection.connect")
    @patch("Connection.Connection.do_handshake")
    def test_broadcast_queue_operations(self, mock_handshake, mock_connect):
        ai = Connection("127.0.0.1", 4242, "TeamA")

        self.assertEqual(ai.broadcast_count(), 0)
        self.assertIsNone(ai.get_next_broadcast())

        ai.broadcast_queue.append(BroadcastMessage(direction=1, message="hello"))
        ai.broadcast_queue.append(BroadcastMessage(direction=4, message="world"))

        self.assertEqual(ai.broadcast_count(), 2)
        first = ai.get_next_broadcast()
        self.assertEqual(first.direction, 1)
        self.assertEqual(first.message, "hello")
        self.assertEqual(ai.broadcast_count(), 1)

        all_broadcasts = ai.get_broadcasts()
        self.assertEqual(len(all_broadcasts), 1)
        self.assertEqual(all_broadcasts[0].message, "world")
        self.assertEqual(ai.broadcast_count(), 0)

    @patch("Connection.Connection.connect")
    @patch("Connection.Connection.do_handshake")
    def test_event_queue_operations(self, mock_handshake, mock_connect):
        ai = Connection("127.0.0.1", 4242, "TeamA")

        self.assertEqual(ai.event_count(), 0)
        self.assertIsNone(ai.get_next_event())
        ai.event_queue.append(ServerEvent(type="eject", message={"direction": 2}))
        ai.event_queue.append(ServerEvent(type="dead", message={}))

        self.assertEqual(ai.event_count(), 2)
        first = ai.get_next_event()
        self.assertEqual(first.event_type, "eject")
        self.assertEqual(first.data["direction"], 2)
        all_events = ai.get_events()
        self.assertEqual(len(all_events), 1)
        self.assertEqual(all_events[0].event_type, "dead")
        self.assertEqual(ai.event_count(), 0)

    @patch("Connection.Connection.connect")
    @patch("Connection.Connection.do_handshake")
    def test_command_queue_and_sending(self, mock_handshake, mock_connect):
        ai = Connection("127.0.0.1", 4242, "TeamA")
        ai.running = True
        cmd_id = ai.send_command("Look")
        self.assertEqual(cmd_id, 1)
        self.assertEqual(len(ai.command_queue), 1)
        self.assertEqual(ai.command_queue[0].command, "Look")
        with patch.object(ai, "send_raw_command", return_value=True) as mock_send_raw:
            ai.send_cmd_buffer()

            mock_send_raw.assert_called_with("Look")
            self.assertEqual(len(ai.command_queue), 0)
            self.assertIn(1, ai.active_requests)
            self.assertEqual(ai.active_requests[1].command, "Look")

    @patch("Connection.Connection.connect")
    @patch("Connection.Connection.do_handshake")
    def test_process_incoming_responses(self, mock_handshake, mock_connect):
        ai = Connection("127.0.0.1", 4242, "TeamA")
        ai.active_requests[42] = CommandRequest(command="Forward", id=42)
        ai.command_responses.append("ok")
        ai.process_incoming_responses()
        self.assertIn(42, ai.response_buffer)
        self.assertEqual(ai.response_buffer[42].command, "ok")
        self.assertEqual(ai.response_buffer[42].id, 42)


class TestConnectionFunctional(unittest.TestCase):
    def setUp(self):
        self.host = "127.0.0.1"
        self.port = 8889
        self.server_ready = threading.Event()
        self.server_thread = threading.Thread(
            target=self._run_dummy_server, daemon=True
        )
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
        conn.recv(1024)
        conn.sendall(b"1\n")
        conn.sendall(b"10 10\n")

        data = conn.recv(1024)
        if data == b"Forward\n":
            conn.sendall(b"ok\n")

        conn.close()
        server.close()

    def test_full_connection_and_command(self):
        ai = Connection(self.host, self.port, "MyTeam")
        ai.start()
        cmd_id = ai.send_command("Forward")
        self.assertGreater(cmd_id, 0)
        time.sleep(0.2)
        self.assertIn(cmd_id, ai.response_buffer)
        response_obj = ai.response_buffer[cmd_id]
        self.assertEqual(response_obj.command, "ok")
        self.assertEqual(response_obj.id, cmd_id)

        ai.disconnect()


if __name__ == "__main__":
    unittest.main()
