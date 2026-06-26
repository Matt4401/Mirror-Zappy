from ..AState import AState


class LeaderState(AState):
    def execute(self):
        needed_stones = None  # le leader demande 2 linemate, 2 deraumere, 3 sibur, 3mendianes, 2 phiras, 1 thystame

        if not needed_stones:
            pass
            # broadcast pour demander les pierres en 360
        else:
            pass
            # boucler tant qu'on a bouffe (break) et que l'évolution fonctionne
            # checker les autres clients, minimum 8 au bon level
            # État EvolveState
            # get stones -> attention une team peut nous voler entre temps
