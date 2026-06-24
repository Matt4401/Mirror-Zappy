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
            cmd_id = self.trantorian.send_command.look()
            raw_response = self.trantorian.connection.get_command_response(cmd_id)
            if raw_response:
                parsed_tiles = self.trantorian.parser.parse_look(raw_response)
                self.trantorian.player_state.vision.update_tiles(parsed_tiles)
        else:
            stone, tile_index = visible
            self.trantorian.move_to_tile(tile_index)
            self.trantorian.send_command.take_object(stone)
            self.trantorian.refresh_inventory()
            cmd_id = self.trantorian.send_command.look()
            raw_response = self.trantorian.connection.get_command_response(cmd_id)
            if raw_response:
                parsed_tiles = self.trantorian.parser.parse_look(raw_response)
                self.trantorian.player_state.vision.update_tiles(parsed_tiles)
