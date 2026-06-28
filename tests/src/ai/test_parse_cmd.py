import unittest
from unittest.mock import Mock
from src.ParseCommand import ParseCommand

class TestParseCommand(unittest.TestCase):
    def setUp(self):
        self.mock_inventory = Mock()
        self.parser = ParseCommand(self.mock_inventory)

    def test_parse_inventory_valid(self):
        data = "[ food 345, linemate 1, deraumere 2, sibur 3, mendiane 4, phiras 5, thystame 6 ]"
        self.parser.parse_inventory(data)
        self.mock_inventory.update_inventory.assert_called_once_with(1, 2, 3, 4, 5, 6, 345)

    def test_parse_inventory_missing_fields(self):
        data = "[ food 345, linemate 1 ]"
        with self.assertRaises(ValueError):
            self.parser.parse_inventory(data)

    def test_parse_look_valid(self):
        data = "[ player food, , linemate ]"
        result = self.parser.parse_look(data)
        self.assertEqual(result, [["player", "food"], [], ["linemate"]])

    def test_parse_look_empty(self):
        data = "[]"
        result = self.parser.parse_look(data)
        self.assertEqual(result, [[]])