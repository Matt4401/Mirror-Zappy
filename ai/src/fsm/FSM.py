import time
from .Constant import SURVIVAL_THRESHOLD
from .states.AttackState import AttackState
from .states.ReproduceState import ReproduceState
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
        self.pending_commands = {}

    def run(self):
        self.trantorian.logger.warning("===========Start FSM process===========")
        while True:
            meta_cmds = self.tick_manager.tick_update()
            self.send_auto_cmds(meta_cmds)
            # self.process_broadcasts()
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

        self.trantorian.logger.info("[FSM]: Food underfoot, taking it before other actions")
        self.trantorian.take_object("food")
        self.trantorian.refresh_inventory()

    # def process_broadcasts(self): ne pas utiliser pour le moment car class pas push
    #     if not hasattr(self.trantorian, "broadcast_manager"):
    #         return
    #
    #     broadcasts = self.trantorian.connection.get_broadcasts()
    #     for broadcast in broadcasts:
    #         self.trantorian.broadcast_manager.handle_raw(
    #             broadcast, self.tick_manager.tick
    #         )

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
                    print(f"Eror with {cmd_type}: {e}")
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

            # elif cmd is None and hasattr(self.trantorian, "broadcast_manager"):
            #     self.trantorian.logger.info("[FSM]: Auto command Broadcast call")
            #     msg = self.trantorian.broadcast_manager.build_message()
            #     cmd_id = self.trantorian.send_command.broadcast(msg)
            #     self.pending_commands[cmd_id] = "broadcast"

    def update_state(self):
        food = self.trantorian.player_state.inventory.get_food()
        self.trantorian.logger.info(f"[FSM]: number of food : {food}")
        tick = self.tick_manager.tick

        if food < SURVIVAL_THRESHOLD:
            self.trantorian.logger.warning("[FSM]: Food is low, transitioning to SurviveState")
            self.transition_to(SurviveState)
            return

        if self.trantorian.has_enough_resources_for(
                self.trantorian.player_state.level + 1
        ):
            self.trantorian.logger.warning("[FSM]: Enough stones for next level, transitioning to EvolveState")
            self.transition_to(EvolveState)
        else:
            # Pareil ne pas activer pour le moment.
            # threat = None
            # my_size = 1
            # if hasattr(self.trantorian, "broadcast_manager"):
            #     my_size = self.trantorian.broadcast_manager.my_team_size(tick)
            #     threat = self.trantorian.broadcast_manager.get_threat(my_size, tick)
            #
            # if threat:
            #     self.trantorian.logger.warning("[FSM]: Threat detected, transitioning to AttackState")
            #     self.state = AttackState(self.trantorian, threat.direction)
            #     return
            #
            # if hasattr(self.trantorian, "broadcast_manager") and self.trantorian.broadcast_manager.should_reproduce(
            #         tick):
            #     self.trantorian.logger.warning("[FSM]: Team too small or unknown, transitioning to ReproduceState")
            #     self.transition_to(ReproduceState)
            #     return

            self.trantorian.logger.warning("[FSM]: Not enough stones for next level, transitioning to GatherState")
            self.transition_to(GatherState)

    def transition_to(self, state_class):
        if not isinstance(self.state, state_class):
            self.state = state_class(self.trantorian)

    def execute_state(self):
        self.state.execute()
