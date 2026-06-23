from ..AState import AState


class GatherState(AState):
    def execute(self):
        next_level = self.trantorian.player_state.level + 1
        needed_dict = self.trantorian.get_missing_resources_for(next_level)
        needed_stones = list(needed_dict.keys())

        if not needed_stones:
            return

        visible = None
        for stone in needed_stones:
            tile_index = self.trantorian.player_state.vision.get_tile_index_of(stone)
            if tile_index is not None:
                visible = (stone, tile_index)
                break

        if not visible:
            self.trantorian.send_command.forward()
            look_resp = self.trantorian.send_command.look()
            self.trantorian.vision = look_resp
        else:
            stone, tile_index = visible
            self.trantorian.move_to_tile(tile_index)
            self.trantorian.send_command.take_object(stone)
            resp = self.trantorian.send_command.inventory()
            self.trantorian.player_state.inventory.update_inventory(resp)
            look_resp = self.trantorian.send_command.look()
            self.trantorian.vision = look_resp
