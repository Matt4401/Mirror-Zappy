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

