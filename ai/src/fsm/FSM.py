import time
import re
from .Constant import SURVIVAL_THRESHOLD
from .states.AttackState import AttackState
from .states.ReproduceState import ReproduceState
from .states.EvolveState import EvolveState
from .states.GatherState import GatherState
from .states.SurviveState import SurviveState
from .states.HelpTeamMatesState import HelpTeamMatesState
from .AState import AState
from .TickManager import TickManager
from .states.FollowerState import FollowerState
from .states.LeaderState import LeaderState

status = ["LEADER", "FOLLOWER"]


class FiniteStateMachine:
    def __init__(self, default_state: AState, trantorian, tick_manager: TickManager):
        self.state = default_state
        self.trantorian = trantorian
        self.tick_manager = tick_manager
        self.sender = []
        self.pending_commands = {}

    def run(self):
        self.trantorian.logger.warning("===========Start FSM process===========")
        while True:
            meta_cmds = self.tick_manager.tick_update()
            self.send_auto_cmds(meta_cmds)
            self.process_broadcasts()
            self.process_pending_commands()
            self.eat_current_tile_food()
            self.update_state()
            self.execute_state()
            time.sleep(0.01)

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
        self.trantorian.refresh_inventory()

    def process_broadcasts(self):
        if not hasattr(self.trantorian, "broadcast_manager"):
            return
        # ici rajouter une methode qui va checker si on a le leader le plus récent (plus petit id) en fct, si oui, envoyer un broadcast dans la direction, si non passer en follower
        broadcasts = self.trantorian.connection.get_broadcasts()
        for broadcast in broadcasts:
            decoded = self.trantorian.broadcast_manager.read_broadcast(
                broadcast.message
            )
            if decoded is not None:
                level, state = decoded
                self.trantorian.logger.info(
                    f"[FSM] Broadcast décodé de niveau {level}: {state}"
                )
                self.trantorian.received_broadcasts.append(
                    {
                        "level": level,
                        "state": state,
                        "direction": broadcast.direction,
                        # "tick": self.tick_manager.tick,jsp si nécéssaire
                        # "urgent_food": self.urgentfood
                    }
                )

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

            elif cmd is None and hasattr(self.trantorian, "broadcast_manager"):
                self.trantorian.logger.info("[FSM]: Auto command Broadcast call")
                msg = self.trantorian.broadcast_manager.create_message(
                    self.trantorian.status, "Commande de rassemblement"
                )  # ceci est un exemple
                cmd_id = self.trantorian.send_command.broadcast(msg)
                self.pending_commands[cmd_id] = "broadcast"

    def update_state(self):
        food = self.trantorian.player_state.inventory.get_food()
        self.trantorian.logger.info(f"[FSM]: number of food : {food}")
        tick = self.tick_manager.tick

        if food < SURVIVAL_THRESHOLD:
            self.trantorian.logger.warning(
                "[FSM]: Food is low, transitioning to SurviveState"
            )
            self.transition_to(SurviveState)
            return
        if self.trantorian.player_state.level == 8:
            self.transition_to(HelpTeamMatesState)
            return
        if self.trantorian.have_layed != 1:
            pass  # ici se mettre en mode ponte
        if self.trantorian.status == "FOLLOWER":
            self.transition_to(FollowerState)
        if self.trantorian.status == "LEADER":
            self.transition_to(LeaderState)

    def transition_to(self, state_class):
        if not isinstance(self.state, state_class):
            self.state = state_class(self.trantorian)

    def execute_state(self):
        self.state.execute()

    def process_server_events(self):
        while True:
            if len(self.trantorian.connection.event_queue) < 0:
                continue
            event = self.trantorian.connection.get_next_event()
            if event is None:
                break
            if event.event_type == "dead":
                self.trantorian.logger.warning("dead")
                self.trantorian.connection.running = False
                return
            elif event.event_type == "eject":
                direction = event.data.get("direction")
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
                        self.trantorian.player_state.level = new_level
                        self.trantorian.logger.info(f"up level  {new_level}")
                        cmd_id = self.trantorian.send_command.look()
                        self.pending_commands[cmd_id] = "look"
