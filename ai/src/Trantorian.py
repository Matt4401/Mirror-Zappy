from src.Connection import Connection
from src.PlayerState import PlayerState
from src.SendCommand import SendCommand
from src.ParseCommand import ParseCommand
# from .util.BroadcastMessage import BroadcastMessage
# from .util.BroadcastManager import BroadcastManager
from .fsm.Constant import ELEVATION_REQUIREMENTS
import threading
import math
import logging

class Trantorian:
    def __init__(self, port, host, team_name, player_id):
        self.thread = threading.Lock()
        self.answer_list = []
        self.data_lock = threading.Lock()
        self.team_name = team_name
        self.player_id = player_id
        self.connection = Connection(host, port, team_name)
        self.player_state = PlayerState(team_name)
        self.send_command = SendCommand(self.connection)
        self.parser = ParseCommand(self.player_state.inventory)
        self.logger = logging.getLogger(player_id)
        # self.broadcast_message = BroadcastMessage(self.player_state)
        # self.broadcast_manager = BroadcastManager(self.broadcast_message, self.player_state.team_name)

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

    def refresh_inventory(self):
        cmd_id = self.send_command.inventory()
        if cmd_id == 84:
            return False

        result = self.connection.get_command_response(cmd_id)
        if result:
            success, response_str = result
            if success:
                try:
                    self.parser.parse_inventory(response_str)
                    return True
                except ValueError as e:
                    print(f"erro while parse inventory : {e}")
                return False
            return None
        else:
            print(f"Timeout server didn't answer to the inventory cmd: {cmd_id})")
            return False
