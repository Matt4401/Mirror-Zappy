from ..AState import AState


class LeaderState(AState):
    def execute(self):
        self.trantorian.logger.info("===========Entering Leader state===========")

        target_level = self.trantorian.player_state.level + 1
        missing_resources = self.trantorian.get_missing_resources_for(target_level)

        if missing_resources:
            self.trantorian.logger.info(
                f"[Leader]: Missing resources : {missing_resources}"
            )
            msg = self.trantorian.broadcast_manager.create_message(
                self.trantorian.status, f"need {missing_resources}"
            )
            self.trantorian.send_command.broadcast(msg)
            self.trantorian.logger.info("[Leader]: Broadcast send to require resources")
        else:
            self.trantorian.logger.info("[Leader]: Get all resources!")
            # Vérifier quon a bien le minimum de clients au bon level sur la case
            if self.trantorian.has_enough_resources_for(target_level):
                self.trantorian.logger.info("[Leader]: Start Evolution")
                self.transition_to(
                    EvolveState()
                )  # ça ne marche pas car c'est ds fsm...
            else:
                msg = self.trantorian.broadcast_manager.create_message(
                    self.trantorian.status, "join"
                )
                self.trantorian.send_command.broadcast(msg)
