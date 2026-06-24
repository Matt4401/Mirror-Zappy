from ..AState import AState


class AttackState(AState):
    def __init__(self, trantorian, threat_direction: int):
        super().__init__(trantorian)
        self.threat_direction = threat_direction

    def execute(self):
        vision = self.trantorian.player_state.vision

        players_here = (
            vision.get_tiles()[0].count("player") if vision.get_tiles() else 0
        )

        if players_here > 0:
            self.trantorian.send_command.eject()
            return
        self.trantorian.move_one_step_toward(self.threat_direction)
