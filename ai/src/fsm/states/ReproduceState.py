from ..AState import AState
from ..Constant import FOOD_SAFE_FOR_FORK


class ReproduceState(AState):
    def execute(self):
        self.maybe_eat_food()

        food = self.trantorian.player_state.inventory.get_food()
        if food < FOOD_SAFE_FOR_FORK or not self.trantorian.can_reproduce():
            self.trantorian.logger.warning(
                f"[Reproduce]: cannot fork now (food={food})"
            )
            return

        self.trantorian.send_command.available_team_slots()
        self.trantorian.logger.info("[Reproduce]: Forking a new egg")
        self.trantorian.fork()
        self.trantorian.refresh_inventory()
