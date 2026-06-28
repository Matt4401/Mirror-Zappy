import time
import re
from .Constant import (
    SURVIVAL_THRESHOLD,
    FOOD_SAFE_FOR_FORK,
    ELEVATION_REQUIREMENTS,
    FORK_COOLDOWN_SECONDS,
    FOOD_SAFE_FOR_INCANTATION,
    BROADCAST_EVO_PROCESS,
    BROADCAST_DONE,
)
from .states.AttackState import AttackState
from .states.ReproduceState import ReproduceState
from .states.EvolveState import EvolveState
from .states.GatherState import GatherState
from .states.SurviveState import SurviveState
from .states.JoinIncantationState import JoinIncantationState
from .AState import AState
from .TickManager import TickManager


class FiniteStateMachine:
    def __init__(self, default_state: AState, trantorian, tick_manager: TickManager):
        self.state = default_state
        self.trantorian = trantorian
        self.tick_manager = tick_manager
        self.pending_commands = {}

    def run(self):
        self.trantorian.logger.warning("===========Start FSM process===========")
        while self.trantorian.connection.running:
            meta_cmds = self.tick_manager.tick_update()
            self.send_auto_cmds(meta_cmds)
            self.process_broadcasts()
            self.process_server_events()
            self.process_pending_commands()
            self.eat_current_tile_food()
            self.update_state()
            self.execute_state()
            time.sleep(0.01)
        self.trantorian.logger.warning("===========Player died, FSM stopped===========")
        self.trantorian.connection.disconnect()

    def eat_current_tile_food(self):
        if self.pending_commands:
            return

        tiles = self.trantorian.player_state.vision.get_tiles()
        if not tiles or "food" not in tiles[0]:
            return

        self.trantorian.logger.info(
            "[FSM]: Food underfoot, taking it before other actions"
        )
        self.trantorian.take_object("food")
        self.trantorian.inventory()
        # self.trantorian.refresh_inventory()

    def process_pending_commands(self):
        completed = []

        for cmd_id, cmd_type in list(self.pending_commands.items()):
            result = self.trantorian.connection.get_command_response(
                cmd_id, timeout=0.1
            )
            if result is not None:
                success, response = result
                try:
                    if not success:
                        completed.append(cmd_id)
                        continue
                    if cmd_type == "inventory":
                        self.trantorian.parser.parse_inventory(response)
                    elif cmd_type == "look":
                        tiles = self.trantorian.parser.parse_look(response)
                        self.trantorian.player_state.vision.update_tiles(tiles)
                    completed.append(cmd_id)
                except Exception as e:
                    print(f"Error with {cmd_type}: {e}")
                    completed.append(cmd_id)

        for cmd_id in completed:
            if cmd_id in self.pending_commands:
                del self.pending_commands[cmd_id]

    def send_auto_cmds(self, meta_cmds: list[str]):
        if self.pending_commands:
            return

        for cmd in meta_cmds:
            if cmd == "Inventory":
                self.trantorian.logger.info("[FSM]: Auto command Inventory call")
                cmd_id = self.trantorian.send_command.inventory()
                self.pending_commands[cmd_id] = "inventory"
                # self.trantorian.refresh_inventory()

            elif cmd == "Look":
                self.trantorian.logger.info("[FSM]: command Look call")
                cmd_id = self.trantorian.send_command.look()
                self.pending_commands[cmd_id] = "look"

            elif cmd is None and hasattr(self.trantorian, "broadcast_message"):
                self.trantorian.logger.info("[FSM]: Auto command Broadcast call")
                msg = self.trantorian.broadcast_message.create_message()
                cmd_id = self.trantorian.send_command.broadcast(msg)
                self.pending_commands[cmd_id] = "broadcast"

    def update_state(self):
        ps = self.trantorian.player_state
        food = ps.inventory.get_food()
        self.trantorian.logger.info(f"[FSM]: number of food : {food}")

        if isinstance(self.state, EvolveState):
            if food < SURVIVAL_THRESHOLD:
                self.state.reset("food_critical")
                self.transition_to(SurviveState)
                return
            if ps.is_joining_incantation:
                self.transition_to(JoinIncantationState)
                return
            if not self.state.finished:
                return
            # finished

        if food < SURVIVAL_THRESHOLD:
            self.trantorian.logger.warning(
                "[FSM]: Food is low, transitioning to SurviveState"
            )
            if isinstance(self.state, JoinIncantationState):
                ps.clear_incantation_target()
            self.transition_to(SurviveState)
            return

        if isinstance(self.state, JoinIncantationState):
            if ps.is_joining_incantation and not getattr(
                self.state, "force_exit", False
            ):
                return
            ps.clear_incantation_target()

        if ps.is_joining_incantation:
            self.transition_to(JoinIncantationState)
            return

        if self.trantorian.has_enough_resources_for(ps.level + 1):
            if food >= FOOD_SAFE_FOR_INCANTATION:
                self.trantorian.logger.warning(
                    "[FSM]: Enough stones, transitioning to EvolveState"
                )
                self.transition_to(EvolveState)
                return
            self.transition_to(SurviveState)
            return

        elapsed = time.time() - self.trantorian.last_fork_time
        should_reproduce = (
            self.trantorian.can_reproduce()
            and food >= FOOD_SAFE_FOR_FORK
            and elapsed >= FORK_COOLDOWN_SECONDS
        )
        if self.trantorian.sex == "Female":
            if should_reproduce:
                self.transition_to(ReproduceState)
                return
        self.trantorian.logger.warning(
            "[FSM]: Not enough stones, transitioning to GatherState"
        )
        self.transition_to(GatherState)

    def process_broadcasts(self):
        ps = self.trantorian.player_state
        broadcasts = self.trantorian.connection.get_broadcasts()
        for broadcast in broadcasts:
            direction = broadcast.direction
            message = broadcast.message

            if "|" not in message:
                continue
            team_name, payload = message.split("|", 1)
            if team_name != ps.team_name:
                continue

            parts = payload.split(":")
            kind = parts[0]

            if kind == BROADCAST_EVO_PROCESS:
                try:
                    level = int(parts[1])
                    leader = int(parts[2])
                except (IndexError, ValueError):
                    continue
                self._handle_evo_process(direction, level, leader)

            elif kind == BROADCAST_DONE:
                try:
                    leader = int(parts[2])
                except (IndexError, ValueError):
                    leader = None
                self.handle_done(leader)

    def _handle_evo_process(self, direction, level, leader):
        ps = self.trantorian.player_state

        if level != ps.level:
            return
        if leader == self.trantorian.leader_id:
            return  # its the same player (not appended normally)

        if isinstance(self.state, EvolveState):
            if leader < self.trantorian.leader_id:
                self.state.request(direction, level, leader)
            return

        current = ps.incantation_leader
        if not ps.is_joining_incantation or current is None or leader <= current:
            ps.set_incantation_target(direction, level, leader)
            if isinstance(self.state, JoinIncantationState):
                self.state.on_signal()

    def handle_done(self, leader):
        ps = self.trantorian.player_state
        if not isinstance(self.state, JoinIncantationState):
            return
        if (
            ps.incantation_leader is None
            or leader is None
            or leader == ps.incantation_leader
        ):
            ps.clear_incantation_target()
            self.state.force_exit = True

    def transition_to(self, state_class):
        if not isinstance(self.state, state_class) or getattr(
            self.state, "finished", False
        ):
            self.state = state_class(self.trantorian)

    def execute_state(self):
        self.state.execute()

    def process_broadcast_events(self):
        while True:
            if len(self.trantorian.broadcast_queue) > 0:
                event = self.trantorian.get_next_broadcast()

    def process_server_events(self):
        while True:
            event = self.trantorian.connection.get_next_event()
            if event is None:
                break
            if event.event_type == "dead":
                self.trantorian.logger.warning("dead")
                self.trantorian.connection.running = False
                return
            elif event.event_type == "eject":
                # direction = event.data.get("direction")
                cmd_id = self.trantorian.send_command.look()
                self.pending_commands[cmd_id] = "look"

            elif event.event_type == "elevation":
                msg = event.data.get("message", "")
                self.trantorian.logger.info(f" event -> {msg}")
                if "Elevation underway" in msg:
                    pass
                elif "Current level" in msg:
                    match = re.search(r"Current level:\s*(\d+)", msg)
                    if match:
                        new_level = int(match.group(1))
                        if new_level > self.trantorian.player_state.level:
                            self.trantorian.player_state.level = new_level
                            self.trantorian.player_state.vision.set_level(new_level)
                            self.trantorian.player_state.vision.reset_on_turn()
                            if isinstance(self.state, EvolveState):
                                self.state.stones_placed = False
                                self.state.wait_ticks = 0
                                self.state.finished = True
                            self.trantorian.logger.info(f"up level  {new_level}")
                            cmd_id = self.trantorian.send_command.look()
                            self.pending_commands[cmd_id] = "look"
