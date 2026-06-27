from ..AState import AState


class FollowerState(AState):
    def execute(self):
        self.trantorian.logger.info("===========Entering Follower state===========")
        if not self.trantorian.received_broadcasts:
            self.trantorian.send_command.forward()

        latest_broadcast = self.trantorian.received_broadcasts.pop(0)
        direction = latest_broadcast.get("dir")
        raw_message = latest_broadcast.get("msg")
        decoded = self.trantorian.broadcast_manager.read_broadcast(raw_message)
        if decoded:
            sender_id, instruction = decoded
            if "join" in instruction:
                if direction == 0:
                    self.trantorian.logger.info("[Follower]: On leader emplacement")
                    self.trantorian.broadcast_manager.create_message(
                        self.trantorian.broadcast_manager.status, ""
                    )  # pas besoin d'instruction ici normlaement
                else:
                    self.trantorian.logger.info(
                        f"[Follower]: To leader emplacement (direction {direction})"
                    )
                    self.trantorian.move_one_step_toward(direction)
            elif "drop" in instruction:
                pass  # la faut jeter la pierre demandée + broadast pour informer le leade

        # il manque juste la gestion de la demande du leader pour une pierre, je pense il faut un état au follower genre searchin' stone, giving stone)
