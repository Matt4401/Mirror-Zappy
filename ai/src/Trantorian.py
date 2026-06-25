from src.Connection import Connection
from src.PlayerState import PlayerState
from src.SendCommand import SendCommand
from src.ParseCommand import ParseCommand

from .util.BroadcastMessageManager import BroadcastMessageManager
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
        self.logger = logging.getLogger(f"player_{player_id}")
        self.broadcast_manager = BroadcastMessageManager(self.player_state)
        self.received_broadcasts = []

    def wait_for_response(self, cmd_id, timeout=5.0):
        if cmd_id in (None, 84):
            return None
        return self.connection.get_command_response(cmd_id, timeout=timeout)

    def move_one_step_toward(self, threat_direction):
        if threat_direction == 7:
            self.connection.turn_right()
        if threat_direction == 3:
            self.connection.turn_left()
        if threat_direction == 5:
            self.connection.turn_right()
            self.connection.turn_right()
        if threat_direction == 2:
            self.connection.turn_left()
            self.connection.forward()
            self.connection.turn_right()
        if threat_direction == 8:
            self.connection.turn_right()
            self.connection.forward()
            self.connection.turn_left()
        if threat_direction == 6:
            self.connection.turn_right()
            self.connection.forward()
            self.connection.turn_right()
        if threat_direction == 4:
            self.connection.turn_left()
            self.connection.forward()
            self.connection.turn_left()
        self.connection.forward()

    def invalidate_vision(self):
        self.player_state.vision.reset_on_turn()
        # self.player_state.vision = []
        # self.player_state.vision.current_level = self.player_state.level

    def turn(self, cmd_fn, direction_delta):
        cmd_id = cmd_fn()
        result = self.wait_for_response(cmd_id)
        if result and result[0]:
            new_direction = (
                (self.player_state.get_direction() - 1 + direction_delta) % 8
            ) + 1
            self.player_state.update_direction(new_direction)
            self.invalidate_vision()
        return result

    def forward(self):
        cmd_id = self.send_command.forward()
        result = self.wait_for_response(cmd_id)
        if result and result[0]:
            self.player_state.vision.shift_on_forward()
        return result

    def turn_right(self):
        return self.turn(self.send_command.right, 1)

    def turn_left(self):
        return self.turn(self.send_command.left, -1)

    def look(self):
        cmd_id = self.send_command.look()
        result = self.wait_for_response(cmd_id)
        if result:
            success, response_str = result
            if success:
                self.player_state.vision.update_tiles(
                    self.parser.parse_look(response_str)
                )
        return result

    def inventory(self):
        cmd_id = self.send_command.inventory()
        result = self.wait_for_response(cmd_id)
        if result:
            success, response_str = result
            if success:
                self.parser.parse_inventory(response_str)
        return result

    def take_object(self, obj):
        cmd_id = self.send_command.take_object(obj)
        result = self.wait_for_response(cmd_id)
        if result and result[0]:
            self.invalidate_vision()
        return result

    def set_object_down(self, obj):
        cmd_id = self.send_command.set_object_down(obj)
        result = self.wait_for_response(cmd_id)
        if result and result[0]:
            self.invalidate_vision()
        return result

    def start_incantation(self):
        cmd_id = self.send_command.start_incantation()
        return self.wait_for_response(cmd_id)

    def move_to_tile(self, index):
        if index == 0:
            return
        row = int(math.sqrt(index))
        start_of_level = row * row
        dx = index - (start_of_level + row)
        for _ in range(row):
            self.forward()
        if dx < 0:
            self.turn_left()
            for _ in range(abs(dx)):
                self.forward()
        elif dx > 0:
            self.turn_right()
            for _ in range(dx):
                self.forward()

    def has_enough_resources_for(self, target_level: int) -> bool:
        if target_level not in ELEVATION_REQUIREMENTS:
            return False

        requirements = ELEVATION_REQUIREMENTS[target_level]

        for resource, required_amount in requirements.items():
            if resource == "player":
                tiles = self.player_state.vision.get_tiles()
                if not tiles:
                    return False
                players_on_tile = tiles[0].count("player")
                if players_on_tile < required_amount:
                    return False
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
        result = self.inventory()
        if result is None:
            return False

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
            print("Timeout server didn't answer to the inventory cmd")
            return False
