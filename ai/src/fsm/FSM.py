from Constant import SURVIVAL_THRESHOLD
# from ai.src.fsm.states.AttackState import AttackState
# from ai.src.fsm.states.ReproduceState import ReproduceState
from ai.src.fsm.states.EvolveState import EvolveState
from ai.src.fsm.states.GatherState import GatherState
from ai.src.fsm.states.SurviveState import SurviveState
from AState import AState


class FiniteStateMachine:
    def __init__(self, default_state: AState, trantorian):
        self.state = default_state
        self.trantorian = trantorian
        self.sender = []

    def run(self):
        while True:
            self.trantorian.inventory = self.trantorian.connexion.send_command("Inventory")  # a changer
            self.update_state()
            self.execute_state()

    def update_state(self):
        food = self.trantorian.inventory.get_food()

        if food < SURVIVAL_THRESHOLD:
            self.transition_to(SurviveState)

        elif not isinstance(self.state, SurviveState):
            if self.trantorian.has_all_stones():  # methode a faire
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
