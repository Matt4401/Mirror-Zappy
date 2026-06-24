from ..AState import AState


class SurviveState(AState):
    def execute(self):
        closest_food_idx = self.trantorian.player_state.vision.get_tile_index_of("food")

        if closest_food_idx is not None:
            self.trantorian.move_to_tile(closest_food_idx)
            self.trantorian.send_command.take_object("food")
            self.trantorian.refresh_inventory()

        else:
            cmd_id = self.trantorian.send_command.look()
            raw_response = self.trantorian.connection.get_command_response(cmd_id)
            if raw_response:
                parsed_tiles = self.trantorian.parser.parse_look(raw_response)
                self.trantorian.player_state.vision.update_tiles(parsed_tiles)

        closest_food_idx = self.trantorian.player_state.vision.get_tile_index_of("food")
        if closest_food_idx is None:
            self.trantorian.send_command.forward()
            cmd_id = self.trantorian.send_command.look()
            raw_response = self.trantorian.connection.get_command_response(cmd_id)
            if raw_response:
                parsed_tiles = self.trantorian.parser.parse_look(raw_response)
                self.trantorian.player_state.vision.update_tiles(parsed_tiles)
