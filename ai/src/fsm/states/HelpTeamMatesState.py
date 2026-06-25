from ..AState import AState


class HelpTeamMatesState(AState):
    def __init__(self, trantorian):
        super().__init__(trantorian)
        self.trantorian.logger.info(
            "===========Entering Help Team mates state==========="
        )
        self.target_direction = 0

    def execute(self):
        while len(self.trantorian.received_broadcasts) > 0:
            bc = self.trantorian.received_broadcasts.pop(0)
            if bc["level"] != 8:
                self.target_direction = bc["direction"]
                self.trantorian.logger.info(
                    f"[HelpTeamMates] We found a team mate via FSM logs"
                )
                break
        if self.target_direction > 0:
            self.trantorian.logger.info(
                f"[HelpTeamMates] going to {self.target_direction}"
            )
            self.trantorian.move_one_step_toward(self.target_direction)
            self.target_direction = 0

        elif self.target_direction == 0:
            self.trantorian.logger.info("[HelpTeamMates] Don't move")
