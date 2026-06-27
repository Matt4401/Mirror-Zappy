from ..AState import AState

class LeaderState(AState):
    def execute(self):
        self.trantorian.logger.info("===========Entering Leader state===========")

        target_level = self.trantorian.player_state.level + 1
        missing_resources = self.trantorian.get_missing_resources_for_list(target_level)

        if missing_resources:
            for stone in missing_resources:
                if self.trantorian.player_state.vision.get_tile_index_of(stone) == 0:
                    self.trantorian.take_object(stone)
                    self.trantorian.refresh_inventory()

            self.trantorian.logger.info(
                f"[Leader]: Missing resources : {missing_resources}"
            )

            resources_str = " ".join(missing_resources)

            msg = self.trantorian.broadcast_manager.create_message(
                self.trantorian.status, f"need {resources_str}"
            )
            self.trantorian.send_command.broadcast(msg)
            self.trantorian.logger.info("[Leader]: Broadcast send to require resources")
        else:
            self.trantorian.logger.info("[Leader]: Get all resources!")
            if self.trantorian.has_enough_resources_for(target_level):
                self.trantorian.logger.info("[Leader]: Start Evolution")
                self.fsm.transition_to(EvolveState)
            else:
                msg = self.trantorian.broadcast_manager.create_message(
                    self.trantorian.status, "join"
                )
                self.trantorian.send_command.broadcast(msg)