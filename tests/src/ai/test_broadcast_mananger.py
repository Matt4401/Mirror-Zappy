import unittest
from unittest.mock import Mock
from src.util.BroadcastMessageManager import BroadcastMessage


class TestBroadcastMessageManager(unittest.TestCase):
    def setUp(self):
        self.mock_player_state = Mock()
        self.mock_player_state.team_name = "TeamA"
        self.mock_player_state.level = 2
        self.manager = BroadcastMessage(self.mock_player_state)


    def test_code_decode(self):
        message = "HELLO WORLD"
        encoded = self.manager.code(message)
        decoded = self.manager.decode(encoded)
        self.assertEqual(decoded, message)

    def test_create_message(self):
        msg = self.manager.create_message()
        self.assertTrue(msg.startswith("TeamA_"))

    def test_read_broadcast_valid(self):
        created_msg = self.manager.create_message()
        parsed = self.manager.read_broadcast(created_msg)
        self.assertIsNotNone(parsed)

    def test_read_broadcast_invalid_team(self):
        created_msg = "TeamB_123-123-123"
        parsed = self.manager.read_broadcast(created_msg)
        self.assertIsNone(parsed)

    def test_read_broadcast_malformed(self):
        parsed = self.manager.read_broadcast("RANDOM_GARBAGE_STRING")
        self.assertIsNone(parsed)
