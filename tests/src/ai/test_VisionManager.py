import unittest
from src.util.VisionManager import VisionManager

class TestVisionManager(unittest.TestCase):
    def setUp(self):
        self.vision = VisionManager()

    def test_initial_state(self):
        self.assertEqual(self.vision.current_level, 1)
        self.assertEqual(self.vision.tiles, [])

    def test_set_level(self):
        self.vision.set_level(3)
        self.assertEqual(self.vision.current_level, 3)

    def test_update_tiles(self):
        new_tiles = [["player"], ["food", "linemate"]]
        self.vision.update_tiles(new_tiles)
        self.assertEqual(self.vision.get_tiles(), new_tiles)

    def test_remove_resource_underfoot(self):
        self.vision.update_tiles([["food", "linemate"]])
        self.vision.remove_resource_underfoot("food")
        self.assertIn("linemate", self.vision.get_tiles()[0])
        self.assertNotIn("food", self.vision.get_tiles()[0])

    def test_add_resource_underfoot(self):
        self.vision.update_tiles([["linemate"]])
        self.vision.add_resource_underfoot("food")
        self.assertIn("food", self.vision.get_tiles()[0])
        self.assertIn("linemate", self.vision.get_tiles()[0])

    def test_get_tile_index_of_found(self):
        self.vision.update_tiles([["player"], [], ["food"]])
        self.assertEqual(self.vision.get_tile_index_of("food"), 2)

    def test_get_tile_index_of_not_found(self):
        self.vision.update_tiles([["player"], [], ["food"]])
        self.assertIsNone(self.vision.get_tile_index_of("sibur"))

    def test_reset_on_turn(self):
        self.vision.update_tiles([["food"]])
        self.vision.reset_on_turn()
        self.assertEqual(self.vision.get_tiles(), [])