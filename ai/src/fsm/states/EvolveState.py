from ..AState import AState
from ..Constant import ELEVATION_REQUIREMENTS


class EvolveState(AState):
    def execute(self):
        self.trantorian.logger.info("===========Entering Evolve state===========")
        level = self.trantorian.player_state.level
        req = ELEVATION_REQUIREMENTS.get(level + 1, {})

        for resource, amount in req.items():
            if resource == "player":
                continue
            for _ in range(amount):
                res = self.trantorian.set_object_down(resource)
                if res and res[0]:
                    val = getattr(self.trantorian.player_state.inventory, resource, 0)
                    setattr(self.trantorian.player_state.inventory, resource, val - 1)

        result = self.trantorian.start_incantation()
        if result:
            success, response = result
            if success and "Current level" in response:
                self.trantorian.logger.warning(
                    "[Evolve]: Incantation successful, leveling up!"
                )
                self.trantorian.player_state.upgrade_level()
                self.trantorian.player_state.vision.current_level += 1
                self.trantorian.refresh_inventory()
                # for stone in resource:
                #   self.trantorian.take_object(stone)
