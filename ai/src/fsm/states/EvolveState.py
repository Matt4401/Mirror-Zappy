from ..AState import AState
from ..Constant import ELEVATION_REQUIREMENTS


class EvolveState(AState):
    def execute(self):
        level = self.trantorian.player_state.level
        req = ELEVATION_REQUIREMENTS.get(level + 1, {})

        for resource, amount in req.items():
            if resource == "player":
                continue
            for _ in range(amount):
                self.trantorian.send_command.set_object(resource)

        resp = self.trantorian.send_command.start_incantation()

        if resp and "Current level" in resp:
            self.trantorian.player_state.upgrade_level()
            self.trantorian.vision.current_level += 1
            inv_resp = self.trantorian.send_command.inventory()
            self.trantorian.player_state.inventory.update_inventory(*inv_resp)
