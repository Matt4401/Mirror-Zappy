from src.Connection import Connection
from src.PlayerState import PlayerState
from src.SendCommand import SendCommand
import threading
import math

ELEVATION_REQUIREMENTS = {
    2: {
        "player": 1,
        "linemate": 1,
        "deraumere": 0,
        "sibur": 0,
        "phiras": 0,
        "thystame": 0,
    },
    3: {
        "player": 2,
        "linemate": 1,
        "deraumere": 1,
        "sibur": 1,
        "phiras": 0,
        "thystame": 0,
    },
    4: {
        "player": 2,
        "linemate": 2,
        "deraumere": 0,
        "sibur": 1,
        "phiras": 2,
        "thystame": 0,
    },
    5: {
        "player": 4,
        "linemate": 1,
        "deraumere": 2,
        "sibur": 1,
        "phiras": 3,
        "thystame": 0,
    },
    6: {
        "player": 4,
        "linemate": 1,
        "deraumere": 2,
        "sibur": 3,
        "phiras": 0,
        "thystame": 1,
    },
    7: {
        "player": 6,
        "linemate": 1,
        "deraumere": 2,
        "sibur": 3,
        "phiras": 1,
        "thystame": 0,
    },
    8: {
        "player": 6,
        "linemate": 2,
        "deraumere": 2,
        "sibur": 2,
        "phiras": 2,
        "thystame": 1,
    },
}


class Trantorian:
    def __init__(self, port, host, team_name):
        self.thread = threading.Lock()
        self.answer_list = []
        self.data_lock = threading.Lock()
        self.team_name = team_name
        self.connection = Connection(host, port, team_name)
        self.player_state = PlayerState(team_name)
        self.send_command = SendCommand(self.connection)

    def move_to_tile(self, index):
        if index == 0:
            return
        n = int(math.sqrt(index))
        start_of_level = n * n
        dx = index - (start_of_level + n)
        for _ in range(n):
            self.send_command.forward()
        if dx < 0:
            for _ in range(abs(dx)):
                self.send_command.left()
        elif dx > 0:
            for _ in range(dx):
                self.send_command.right()

    def has_enough_resources_for(self, target_level: int) -> bool:
        if target_level not in ELEVATION_REQUIREMENTS:
            return False

        requirements = ELEVATION_REQUIREMENTS[target_level]

        for resource, required_amount in requirements.items():
            if resource == "player":
                continue
            current_amount = getattr(self.player_state.inventory, resource, 0)
            if current_amount < required_amount:
                return False

        return True

    def get_missing_resources_for(self, target_level: int) -> dict:
        missing = {}
        if target_level not in ELEVATION_REQUIREMENTS:
            return missing

        requirements = ELEVATION_REQUIREMENTS[target_level]
        for resource, required_amount in requirements.items():
            if resource == "player":
                continue
            current_amount = getattr(self.player_state.inventory, resource, 0)
            if current_amount < required_amount:
                missing[resource] = required_amount - current_amount

        return missing
