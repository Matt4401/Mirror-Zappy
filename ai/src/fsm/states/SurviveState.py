from ..AState import AState


class SurviveState(AState):
    def execute(self):
        closest_food_idx = self.trantorian.vision.get_tile_index_of("food")

        if closest_food_idx is not None:
            # self.trantorian.move_to_tile(closest_food_idx) # Fct de pathfinding de noah (pas encore push)
            self.trantorian.send_command.take_object("food")

        else:
            look_resp = self.trantorian.send_command.look()
            self.trantorian.vision = look_resp

        closest_food_idx = self.trantorian.vision.get_tile_index_of("food")
        if closest_food_idx is None:
            self.trantorian.send_command.forward()
