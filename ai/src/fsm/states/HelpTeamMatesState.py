from ..AState import AState
from src.util.BroadcastMessage import BroadcastMessage


class HelpTeamMatesState(AState):
    def __init__(self, trantorian):
        super().__init__(trantorian)
        self.trantorian.logger.info(
            "===========Entering Help Team mates state==========="
        )
        self.broadcast_decoder = BroadcastMessage(self.trantorian.player_state)
        self.target_direction = 0

    def execute(self):
        while len(self.trantorian.broadcast_queue) > 0:
            event = self.trantorian.get_next_broadcast()
            if event == None:
                break
            direction = event.direction
            raw_message = event.message
            decoded = self.broadcast_decoder.read_broadcast(raw_message)
            if decoded is not None:
                level, command = decoded
                if level == 8:
                    break
                self.target_direction = direction
                self.trantorian.logger.info(f"[HelpTeamMates] Wer found a team mate")
                break
        if self.target_direction > 0:
            self.trantorian.logger.info(
                f"[HelpTeamMates] going to {self.target_direction}"
            )
            self.trantorian.move_one_step_toward(self.target_direction)
            self.target_direction = 0

        elif self.target_direction == 0:
            self.trantorian.logger.info("[HelpTeamMates] Don't move")
