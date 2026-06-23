from ..AState import AState
from ai.src.fsm.states.EvolveState import EvolveState


class GatherState(AState):
    def execute(self):
        next_level = self.trantorian.player_state.level + 1
        needed_dict = self.trantorian.get_missing_resources_for(next_level)
        needed_stones = list(needed_dict.keys())

        if not needed_stones:
            return EvolveState(self.trantorian)

        visible = self.trantorian.player_state.vision.find_any(needed_stones)

        if not visible:
            self.trantorian.send_command.forward()
        else:
            stone, tile_index = visible[0]

            if tile_index == 0:
                self.trantorian.send_command.take(stone)
                resp = self.trantorian.send_command.inventory()
                self.trantorian.player_state.inventory.update_inventory(resp)
            else:
                self.trantorian.move_to_tile(tile_index)
        return self
