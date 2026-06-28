import unittest
from unittest.mock import patch, MagicMock
from src.Trantorian import Trantorian

class TestTrantorian(unittest.TestCase):
    @patch("src.Trantorian.Connection")
    @patch("src.Trantorian.PlayerState")
    @patch("src.Trantorian.BroadcastMessageManager")
    def setUp(self, mock_broadcast, mock_player, mock_connection):
        self.mock_conn_instance = MagicMock()
        mock_connection.return_value = self.mock_conn_instance
        self.trantorian = Trantorian(1234, "localhost", "TeamA", "player_1")
        self.trantorian.send_command = MagicMock()

    def test_wait_for_response(self):
        self.mock_conn_instance.get_command_response.return_value = (True, "ok")
        result = self.trantorian.wait_for_response(1)
        self.assertEqual(result, (True, "ok"))

    def test_wait_for_response_invalid_id(self):
        self.assertIsNone(self.trantorian.wait_for_response(84))
        self.assertIsNone(self.trantorian.wait_for_response(None))

    def test_forward(self):
        self.trantorian.send_command.forward.return_value = 1
        self.mock_conn_instance.get_command_response.return_value = (True, "ok")

        result = self.trantorian.forward()
        self.assertEqual(result, (True, "ok"))
        self.trantorian.player_state.vision.shift_on_forward.assert_called_once()

    def test_turn_right(self):
        self.trantorian.send_command.right.return_value = 1
        self.mock_conn_instance.get_command_response.return_value = (True, "ok")
        self.trantorian.player_state.get_direction.return_value = 1

        result = self.trantorian.turn_right()
        self.assertEqual(result, (True, "ok"))
        self.trantorian.player_state.update_direction.assert_called_once_with(2)

    def test_take_object(self):
        self.trantorian.send_command.take_object.return_value = 1
        self.mock_conn_instance.get_command_response.return_value = (True, "ok")

        result = self.trantorian.take_object("food")
        self.assertEqual(result, (True, "ok"))
        self.trantorian.player_state.vision.reset_on_turn.assert_called_once()