from ..AState import AState


class SurviveState(AState):
    def execute(self):
        self.trantorian.logger.info("===========Entering Survive state===========")
        closest_food_idx = self.trantorian.player_state.vision.get_tile_index_of("food")

        if closest_food_idx is not None:
            self.trantorian.logger.info(
                "[Survive]: Food visible, go to the tile and take it"
            )
            self.trantorian.move_to_tile(closest_food_idx)
            self.trantorian.take_object("food")
            self.trantorian.refresh_inventory()
            self.trantorian.look()

        else:
            self.trantorian.logger.info(
                "[Survive]: No food visible, go forward and look"
            )
            self.trantorian.look()

        closest_food_idx = self.trantorian.player_state.vision.get_tile_index_of("food")
        if closest_food_idx is None:
            self.trantorian.logger.info(
                "[Survive]: No food visible, go forward and look"
            )
            self.trantorian.forward()
            self.trantorian.look()
