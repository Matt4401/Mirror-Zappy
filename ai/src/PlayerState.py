from src.util.InventoryClass import Inventory
from src.util.VisionManager import VisionManager
import random


class PlayerState:
    def __init__(self, team_name):
        self.level = 1
        self.team_name = team_name
        self.direction = random.randint(1, 8)
        self.vision = VisionManager()
        self.inventory = Inventory()

    def get_direction(self):
        return self.direction

    def update_direction(self, new_direction):
        self.direction = new_direction

    def get_vision(self):
        return self.vision

    def update_vision(self, new_vision):
        self.vision = new_vision

    def get_inventory(self):
        return self.inventory

    def update_inventory(self, new_inventory):
        self.inventory = new_inventory

    def get_food(self):
        return self.inventory.get_food()

    def upgrade_level(self):
        self.level += 1
        self.vision.set_level(self.level)

    def get_level(self):
        return self.level
