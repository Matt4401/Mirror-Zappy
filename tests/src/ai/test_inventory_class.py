import unittest
from src.util.InventoryClass import Inventory


class TestInventory(unittest.TestCase):
    def setUp(self):
        self.inventory = Inventory()

    def test_initial_values(self):
        self.assertEqual(self.inventory.food, 10)
        self.assertEqual(self.inventory.linemate, 0)
        self.assertEqual(self.inventory.deraumere, 0)
        self.assertEqual(self.inventory.sibur, 0)
        self.assertEqual(self.inventory.mendiane, 0)
        self.assertEqual(self.inventory.phiras, 0)
        self.assertEqual(self.inventory.thystame, 0)

    def test_update_inventory(self):
        self.inventory.update_inventory(1, 2, 3, 4, 5, 6, 20)
        self.assertEqual(self.inventory.linemate, 1)
        self.assertEqual(self.inventory.deraumere, 2)
        self.assertEqual(self.inventory.sibur, 3)
        self.assertEqual(self.inventory.mendiane, 4)
        self.assertEqual(self.inventory.phiras, 5)
        self.assertEqual(self.inventory.thystame, 6)
        self.assertEqual(self.inventory.food, 20)

    def test_get_food(self):
        self.inventory.food = 15
        self.assertEqual(self.inventory.get_food(), 15)
