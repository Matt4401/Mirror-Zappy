from ..AState import AState


class SurviveState(AState):
    def execute(self):
        closest_food_idx = self.trantorian.vision.get_tile_index_of("food")

        if closest_food_idx is not None:
            self.trantorian.move_to_tile(closest_food_idx)
            self.trantorian.send_command.take_object("food")
            self.trantorian.send_command.inventory()

        else:
            self.trantorian.send_command.look()

        closest_food_idx = self.trantorian.vision.get_tile_index_of("food")
        if closest_food_idx is None:
            self.trantorian.send_command.forward()
            self.trantorian.send_command.look()
