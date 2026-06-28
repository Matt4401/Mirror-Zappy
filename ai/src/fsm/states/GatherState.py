from ..AState import AState
from ..Constant import SURVIVAL_THRESHOLD, FOOD_OPPORTUNISTIC


class GatherState(AState):
    def execute(self):
        self.maybe_eat_food()
        self.trantorian.logger.info("===========Entering Gather state===========")
        food = self.trantorian.player_state.inventory.get_food()
        next_level = self.trantorian.player_state.level + 1
        needed_dict = self.trantorian.get_missing_resources_for(next_level)
        needed_stones = list(needed_dict.keys())

        if not needed_stones:
            self.trantorian.logger.info(
                "[Gather]: Player already get all his needed stones"
            )
            return

        if food < FOOD_OPPORTUNISTIC:
            food_idx = self.trantorian.player_state.vision.get_tile_index_of("food")
            if food_idx is not None:
                self.trantorian.move_to_tile(food_idx)
                self.trantorian.take_object("food")
                self.trantorian.inventory()
                self.trantorian.look()
                return

        visible = None
        for stone in needed_stones:
            tile_index = self.trantorian.player_state.vision.get_tile_index_of(stone)
            if tile_index is not None:
                visible = (stone, tile_index)
                break

        if not visible:
            self.trantorian.logger.info(
                "[Gather]: Not stones in vision, go forward and look"
            )
            self.trantorian.forward()
            self.trantorian.look()
        else:
            self.trantorian.logger.info(
                "[Gather]: Stones visibles, go to the tile and take it"
            )
            stone, tile_index = visible
            self.trantorian.move_to_tile(tile_index)
            self.trantorian.take_object(stone)
            tiles = self.trantorian.player_state.vision.get_tiles()
            if tiles and "food" in tiles[0] and food < FOOD_OPPORTUNISTIC:
                self.trantorian.take_object("food")
            self.trantorian.inventory()
            self.trantorian.look()
