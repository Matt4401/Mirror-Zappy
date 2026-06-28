import unittest
from unittest.mock import Mock
from src.PlayerState import PlayerState


class TestPlayerState(unittest.TestCase):
    def setUp(self):
        self.state = PlayerState("TeamRocket")

    def test_initial_values(self):
        self.assertEqual(self.state.team_name, "TeamRocket")
        self.assertEqual(self.state.level, 1)
        self.assertEqual(self.state.direction, 1)
        self.assertIsNotNone(self.state.inventory)
        self.assertIsNotNone(self.state.vision)

    def test_update_direction(self):
        self.state.update_direction(4)
        self.assertEqual(self.state.get_direction(), 4)

    def test_update_inventory(self):
        mock_inventory = Mock()
        self.state.update_inventory(mock_inventory)
        self.assertEqual(self.state.get_inventory(), mock_inventory)

    def test_get_food(self):
        self.state.inventory.food = 25
        self.assertEqual(self.state.get_food(), 25)

    def test_upgrade_level(self):
        self.state.upgrade_level()
        self.assertEqual(self.state.get_level(), 2)
