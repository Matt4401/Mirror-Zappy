## AttackState

### The AttackState manages the Trantorian's reactions when facing an identified threat.

- First, the Trantorian checks for the presence of other players on its current tile using its vision.

- If one or more players are detected on the tile, it executes the command to eject them.

-  If there are no players on the tile, it moves one step toward the threat's direction.

```python

class AttackState(AState):
    def __init__(self, trantorian, threat_direction: int):
        super().__init__(trantorian)
        self.threat_direction = threat_direction

    def execute(self):
        self.maybe_eat_food()
        vision = self.trantorian.player_state.vision

        players_here = (
            vision.get_tiles()[0].count("player") if vision.get_tiles() else 0
        )

        if players_here > 0:
            self.trantorian.send_command.eject()
            return
        self.trantorian.move_one_step_toward(self.threat_direction)

```