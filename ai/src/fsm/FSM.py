import time
from .Constant import SURVIVAL_THRESHOLD

# from .states.AttackState import AttackState
# from .states.ReproduceState import ReproduceState
from .states.EvolveState import EvolveState
from .states.GatherState import GatherState
from .states.SurviveState import SurviveState
from .AState import AState
from .TickManager import TickManager


class FiniteStateMachine:
    def __init__(self, default_state: AState, trantorian, tick_manager: TickManager):
        self.state = default_state
        self.trantorian = trantorian
        self.tick_manager = tick_manager
        self.sender = []

    def run(self):
        while True:
            meta_cmds = self.tick_manager.tick_update()
            self.send_auto_cmds(meta_cmds)
            self.update_state()
            self.execute_state()
            time.sleep(0.01)

    def send_auto_cmds(self, meta_cmds: list[str]):
        for cmd in meta_cmds:
            if cmd == "Inventory":
                resp = self.trantorian.send_command.inventory

            elif cmd == "Look":
                resp = self.trantorian.send_command.look
                self.trantorian.vision.update_tiles(resp)

            elif cmd is None:
                return
                # lancer un broad cast

    def update_state(self):
        self.trantorian.refresh_inventory()
        food = self.trantorian.player_state.inventory.get_food()

        if food < SURVIVAL_THRESHOLD:
            self.transition_to(SurviveState)
            return

        if self.trantorian.has_enough_resources_for(
            self.trantorian.player_state.level + 1
        ):
            self.transition_to(EvolveState)

        else:
            # broadcast_resp= self.sender.send_cmd("Broadcast")
            # if dangereuse team avec ennemis proche (choisir un seuil):
            #     self.transition_to(AttackState)
            # else if notre team pas assez de memebre (choisir un seuil)
            #     self.transition_to(ReproduceState)
            # else:
            self.transition_to(GatherState)

    def transition_to(self, state_class):
        if not isinstance(self.state, state_class):
            self.state = state_class(self.trantorian)

    def execute_state(self):
        self.state.execute()
