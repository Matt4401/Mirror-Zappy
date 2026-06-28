from src.Connection import Connection
from src.PlayerState import PlayerState
from src.SendCommand import SendCommand
from src.ParseCommand import ParseCommand

from .util.BroadcastMessage import BroadcastMessage
from .fsm.Constant import ELEVATION_REQUIREMENTS, MAX_REPRODUCE_GEN, LIFETIME_FORK_CAP
import threading
import subprocess
import sys
import os
import time
import math
import logging


class Trantorian:
    def __init__(self, port, host, team_name, player_id, connect_timeout=0.0):
        self.thread = threading.Lock()
        self.answer_list = []
        self.data_lock = threading.Lock()
        self.team_name = team_name
        self.player_id = player_id
        self.port = port
        self.host = host
        self.leader_id = sum(ord(c) for c in str(player_id))
        self.connection = Connection(host, port, team_name, connect_timeout)
        self.player_state = PlayerState(team_name)
        self.send_command = SendCommand(self.connection)
        self.parser = ParseCommand(self.player_state.inventory)
        self.logger = logging.getLogger(f"player_{player_id}")
        self.broadcast_message = BroadcastMessage(self.player_state)
        self.children = []
        self.last_fork_time = 0.0  # delai
        self.generation = 0
        self.fork_count = 0

    def spawn_child_process(self):
        self.children = [c for c in self.children if c.poll() is None]
        if len(self.children) >= 6:
            return False

        if self.launch_client(self.generation + 1):
            self.logger.info("[Reproduce]: spawned a new AI client to fill the egg")
            return True
        return False

    def launch_client(self, generation: int) -> bool:
        entry = os.path.join(
            os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "zappy_ai.py"
        )
        cmd = [
            sys.executable,
            entry,
            "-p",
            str(self.port),
            "-n",
            self.team_name,
            "-h",
            self.host,
            "-g",
            str(generation),
        ]
        try:
            child = subprocess.Popen(
                cmd,
                stdin=subprocess.DEVNULL,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
                start_new_session=True,
            )
            self.children.append(child)
            return True
        except Exception as e:
            self.logger.warning(f"[Reproduce]: failed to spawn child: {e}")
            return False

    def fill_initial_team(self):
        free_slots = getattr(self.connection, "client_num", 0)
        if free_slots <= 0:
            return
        self.logger.info(f"[Init]: founder filling {free_slots} free team slots")
        for _ in range(free_slots):
            self.launch_client(1)

    def move_one_step_toward(self, direction: int):
        if direction == 0:
            return
        if direction in (1, 2, 8):
            self.forward()
        elif direction in (3, 4):
            self.turn_left()
            self.forward()
        elif direction == 5:
            self.turn_right()
            self.turn_right()
            self.forward()
        elif direction in (6, 7):
            self.turn_right()
            self.forward()

    def wait_for_response(self, cmd_id, timeout=5.0):
        if cmd_id in (None, 84):
            return None
        return self.connection.get_command_response(cmd_id, timeout=timeout)

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
            self.invalidate_vision()
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
        return self.wait_for_response(cmd_id, timeout=30.0)

    def fork(self):
        cmd_id = self.send_command.fork()
        result = self.wait_for_response(cmd_id, timeout=10.0)
        if result and result[0]:
            self.last_fork_time = time.time()
            self.fork_count += 1
            self.spawn_child_process()
            return True
        return False

    def can_reproduce(self):
        return (
            self.generation < MAX_REPRODUCE_GEN and self.fork_count < LIFETIME_FORK_CAP
        )

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
                continue
            current_amount = getattr(self.player_state.inventory, resource, 0)
            if current_amount < required_amount:
                return False

        return True


from src.Connection import Connection
from src.PlayerState import PlayerState
from src.SendCommand import SendCommand
from src.ParseCommand import ParseCommand

from .util.BroadcastMessage import BroadcastMessage
from .fsm.Constant import ELEVATION_REQUIREMENTS, MAX_REPRODUCE_GEN, LIFETIME_FORK_CAP
import threading
import subprocess
import sys
import os
import time
import math
import logging


class Trantorian:
    def __init__(self, port, host, team_name, player_id, connect_timeout=0.0):
        self.thread = threading.Lock()
        self.answer_list = []
        self.data_lock = threading.Lock()
        self.team_name = team_name
        self.player_id = player_id
        self.port = port
        self.host = host
        self.leader_id = sum(ord(c) for c in str(player_id))
        self.connection = Connection(host, port, team_name, connect_timeout)
        self.player_state = PlayerState(team_name)
        self.send_command = SendCommand(self.connection)
        self.parser = ParseCommand(self.player_state.inventory)
        self.logger = logging.getLogger(f"player_{player_id}")
        self.broadcast_message = BroadcastMessage(self.player_state)
        self.children = []
        self.last_fork_time = 0.0  # delai
        self.generation = 0
        self.fork_count = 0

    def spawn_child_process(self):
        self.children = [c for c in self.children if c.poll() is None]
        if len(self.children) >= 6:
            return False

        if self.launch_client(self.generation + 1):
            self.logger.info("[Reproduce]: spawned a new AI client to fill the egg")
            return True
        return False

    def launch_client(self, generation: int) -> bool:
        entry = os.path.join(
            os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "zappy_ai.py"
        )
        cmd = [
            sys.executable,
            entry,
            "-p",
            str(self.port),
            "-n",
            self.team_name,
            "-h",
            self.host,
            "-g",
            str(generation),
        ]
        try:
            child = subprocess.Popen(
                cmd,
                stdin=subprocess.DEVNULL,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
                start_new_session=True,
            )
            self.children.append(child)
            return True
        except Exception as e:
            self.logger.warning(f"[Reproduce]: failed to spawn child: {e}")
            return False

    def fill_initial_team(self):
        free_slots = getattr(self.connection, "client_num", 0)
        if free_slots <= 0:
            return
        self.logger.info(f"[Init]: founder filling {free_slots} free team slots")
        for _ in range(free_slots):
            self.launch_client(1)

    def move_one_step_toward(self, direction: int):
        if direction == 0:
            return
        if direction in (1, 2, 8):
            self.forward()
        elif direction in (3, 4):
            self.turn_left()
            self.forward()
        elif direction == 5:
            self.turn_right()
            self.turn_right()
            self.forward()
        elif direction in (6, 7):
            self.turn_right()
            self.forward()

    def wait_for_response(self, cmd_id, timeout=5.0):
        if cmd_id in (None, 84):
            return None
        return self.connection.get_command_response(cmd_id, timeout=timeout)

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
            self.invalidate_vision()
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
        return self.wait_for_response(cmd_id, timeout=30.0)

    def fork(self):
        cmd_id = self.send_command.fork()
        result = self.wait_for_response(cmd_id, timeout=10.0)
        if result and result[0]:
            self.last_fork_time = time.time()
            self.fork_count += 1
            self.spawn_child_process()
            return True
        return False

    def can_reproduce(self):
        return (
            self.generation < MAX_REPRODUCE_GEN and self.fork_count < LIFETIME_FORK_CAP
        )

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
        return self.inventory()

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
        return self.inventory()
