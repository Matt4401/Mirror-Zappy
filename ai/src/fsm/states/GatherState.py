from ..AState import AState
from .EvolveState import EvolveState


class GatherState(AState):
    def execute(self):
        pass
        """"
        self.trantorian.logger.info("===========Entering Gather state===========")
        next_level = self.trantorian.player_state.level + 1
        needed_dict = self.trantorian.get_missing_resources_for(next_level)
        needed_stones = list(needed_dict.keys())

        if not needed_stones:
            self.trantorian.logger.info(
                "[Gather]: Player already get all his needed stones"
            )
            self.fsm.transition_to(EvolveState)
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
            self.trantorian.refresh_inventory()
            self.trantorian.look()
            """
