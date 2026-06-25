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
                self.trantorian.set_object_down(resource)

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
