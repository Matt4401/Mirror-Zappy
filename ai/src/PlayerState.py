import random
from src.InventoryClass import Inventory


class PlayerState:
    def __init__(self, team_name):
        self.level = 0
        self.team_name = team_name
        self.direction = random.randint(1, 8)
        self.vision = (
            None  # TODO we have to create a norm for the vision in an other issu
        )
        self.inventory = Inventory()
        self.food_remaining = 0

    def get_food(self):
        return self.food_remaining

    def update_food(self, new_food):
        self.food_remaining += new_food

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

    def upgrade_level(self):
        self.level += 1

    def get_level(self):
        return self.level
