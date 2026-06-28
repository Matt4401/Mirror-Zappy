import unittest
from unittest.mock import Mock
from src.SendCommand import SendCommand


class TestSendCommand(unittest.TestCase):
    def setUp(self):
        self.mock_connection = Mock()
        self.mock_connection.send_command.return_value = 99
        self.sender = SendCommand(self.mock_connection)

    def test_forward(self):
        res = self.sender.forward()
        self.assertEqual(res, 99)
        self.mock_connection.send_command.assert_called_once_with("Forward")

    def test_right(self):
        self.sender.right()
        self.mock_connection.send_command.assert_called_once_with("Right")

    def test_broadcast(self):
        self.sender.broadcast("HELLO")
        self.mock_connection.send_command.assert_called_once_with("Broadcast HELLO")

    def test_take_object(self):
        self.sender.take_object("food")
        self.mock_connection.send_command.assert_called_once_with("Take food")

    def test_start_incantation(self):
        self.sender.start_incantation()
        self.mock_connection.send_command.assert_called_once_with("Incantation")
