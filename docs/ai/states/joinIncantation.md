## JoinIncantationState

### The JoinIncantationState allows the Trantorian to join an incantation initiated by another player (the leader).  
- The Trantorian monitors the time elapsed since the last received signal; if the delay exceeds the limit (JOIN_SIGNAL_TIMEOUT), it considers the leader silent and abandons the process. 
- If the target incantation level no longer matches its current level, it immediately leaves the process.  
- If the target direction is 0, it means it is positioned on the gathering tile and waits for the elevation to begin.  
- If the target is distant, it moves one step toward the bearing indicated by the incantation target.  

```python
class JoinIncantationState(AState):
    def __init__(self, trantorian):
        super().__init__(trantorian)
        self.ticks_since_signal = 0
        self.force_exit = False

    def on_signal(self):
        self.ticks_since_signal = 0

    def execute(self):
        self.maybe_eat_food()
        ps = self.trantorian.player_state

        if ps.incantation_level is not None and ps.level != ps.incantation_level:
            self.trantorian.logger.info("[Join]: level changed, leaving evo process")
            ps.clear_incantation_target()
            self.force_exit = True
            return

        self.ticks_since_signal += 1
        if self.ticks_since_signal > JOIN_SIGNAL_TIMEOUT:
            self.trantorian.logger.warning("[Join]: leader went silent, abandoning")
            ps.clear_incantation_target()
            self.force_exit = True
            return

        target = ps.incantation_target

        if target == 0:
            self.trantorian.logger.info("[Join]: on gather tile, waiting for elevation")
            return
        if target is None:
            return # att broadcast

        self.trantorian.logger.info(f"[Join]: stepping toward bearing {target}")
        self.trantorian.move_one_step_toward(target)
        ps.consume_incantation_target()
```