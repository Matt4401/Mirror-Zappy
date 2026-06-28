from abc import ABC, abstractmethod
from .Constant import FOOD_OPPORTUNISTIC


class AState(ABC):
    def __init__(self, trantorian):
        self.trantorian = trantorian

    @abstractmethod
    def execute(self):
        pass

    def maybe_eat_food(self):
        food = self.trantorian.player_state.inventory.get_food()
        if food >= FOOD_OPPORTUNISTIC:
            return
        tiles = self.trantorian.player_state.vision.get_tiles()
        if tiles and "food" in tiles[0]:
            self.trantorian.take_object("food")
            self.trantorian.inventory()
