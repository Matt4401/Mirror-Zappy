from src.util.InventoryClass import Inventory
from .util.VisionManager import VisionManager


class PlayerState:
    def __init__(self, team_name):
        self.level = 1
        self.team_name = team_name
        self.direction = 1
        self.inventory = Inventory()
        self.vision = VisionManager()
        self.incantation_target = None
        self.incantation_level = None
        self.incantation_leader = None
        self.is_joining_incantation = False
        self.is_evolving = False

    def set_incantation_target(self, direction, level=None, leader=None):
        self.incantation_target = direction
        if level is not None:
            self.incantation_level = level
        if leader is not None:
            self.incantation_leader = leader
        self.is_joining_incantation = True

    def consume_incantation_target(self):
        self.incantation_target = None

    def clear_incantation_target(self):
        self.incantation_target = None
        self.incantation_level = None
        self.incantation_leader = None
        self.is_joining_incantation = False

    def set_evolving(self):
        self.is_evolving = True

    def clear_evolving(self):
        self.is_evolving = False

    def get_direction(self):
        return self.direction

    def update_direction(self, new_direction):
        self.direction = new_direction

    def get_inventory(self):
        return self.inventory

    def update_inventory(self, new_inventory):
        self.inventory = new_inventory

    def get_food(self):
        return self.inventory.get_food()

    def upgrade_level(self):
        self.level += 1

    def get_level(self):
        return self.level
