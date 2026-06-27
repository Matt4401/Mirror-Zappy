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
            # self.process_server_events()
            if not self.trantorian.connection.running:
                self.trantorian.logger.warning(
                    "[FSM]: Player is dead. Stopping FSM loop."
                )
                break

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
        """
        Récupère tous les broadcasts de la boîte aux lettres dédiée,
        les stocke pour le FollowerState, et gère l'élection/timeout du Leader.
        """
        # 1. Utiliser la BONNE méthode de Connection.py pour vider la file des broadcasts
        broadcast_messages = self.trantorian.connection.get_broadcasts()

        current_tick = getattr(self.tick_manager, "current_tick", 0)
        my_id = self.trantorian.broadcast_manager.id
        my_level = self.trantorian.player_state.level

        # Si aucun message n'a été reçu à ce tick, on vérifie juste le timeout du leader
        if not broadcast_messages:
            self._handle_leader_timeout(current_tick)
            return

        for broadcast in broadcast_messages:
            # broadcast est un objet BroadcastMessage (avec .direction et .message)
            raw_message = broadcast.message

            # Tenter de décoder le message via le manager
            decoded = self.trantorian.broadcast_manager.read_broadcast(raw_message)
            if decoded is None:
                continue  # Message corrompu ou d'une autre équipe, on l'ignore

            sender_id, content_str = decoded

            # 2. Remplir la liste au format EXACT attendu par FollowerState.py
            self.trantorian.received_broadcasts.append(
                {"direction": broadcast.direction, "msg": raw_message}
            )

            # 3. Logique d'arbitrage Leader / Follower
            # Le contenu ressemble à : "STATUS LEVEL INSTRUCTION" -> ex: "LEADER 1 need linemate"
            parts = content_str.split(" ", 2)
            if len(parts) >= 2:
                sender_status = parts[0]
                try:
                    sender_level = int(parts[1])
                except ValueError:
                    sender_level = my_level
            else:
                sender_status = "UNKNOWN"
                sender_level = my_level

            # On ne compare notre autorité qu'avec des joueurs de MÊME niveau
            if sender_level == my_level:
                # Si un joueur avec un ID plus petit (prioritaire) dit qu'il est LEADER
                if sender_id < my_id and sender_status == "LEADER":
                    if (
                        self.trantorian.status != "FOLLOWER"
                        or self.trantorian.leader_level != sender_id
                    ):
                        self.trantorian.logger.info(
                            f"[FSM] Abdication ! Je deviens FOLLOWER du joueur {sender_id}"
                        )
                        self.trantorian.status = "FOLLOWER"
                        self.trantorian.leader_level = sender_id

                    # On met à jour le moment où on a entendu ce leader pour éviter le timeout
                    self.trantorian.last_leader_tick = current_tick

                # Si c'est notre leader actuel qui parle (peu importe son message), on reset son timeout
                elif sender_id == self.trantorian.leader_level:
                    self.trantorian.last_leader_tick = current_tick

        # 4. Vérification du timeout après avoir traité tous les messages
        self._handle_leader_timeout(current_tick)

    def _handle_leader_timeout(self, current_tick):
        """Repasse en mode LEADER si notre chef ne donne plus de signes de vie."""
        if self.trantorian.status == "FOLLOWER":
            elapsed_ticks = current_tick - self.trantorian.last_leader_tick
            if elapsed_ticks > self.trantorian.LEADER_TIMEOUT_TICKS:
                self.trantorian.logger.warning(
                    f"[FSM] Le Leader {self.trantorian.leader_level} est silencieux depuis {elapsed_ticks} ticks. "
                    f"Je reprends mon autonomie et repasse LEADER."
                )
                self.trantorian.status = "LEADER"
                self.trantorian.leader_level = self.trantorian.broadcast_manager.id

    def falseprocess_broadcasts(self):
        if not hasattr(self.trantorian, "broadcast_manager"):
            return
        # ici rajouter une methode qui va checker si on a le leader le plus récent (plus petit id) en fct, si oui, envoyer un broadcast dans la direction, si non passer en follower
        # puis traiter que les leaders
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

    def update_state(self):
        food = self.trantorian.player_state.inventory.get_food()
        self.trantorian.logger.info(f"[FSM]: number of food : {food}")
        tick = self.tick_manager.tick

        if isinstance(self.state, SurviveState):
            if food < 10:
                return

        if food < SURVIVAL_THRESHOLD:
            self.trantorian.logger.warning(
                "[FSM]: Food is low, transitioning to SurviveState"
            )
            self.transition_to(SurviveState)
            self.trantorian.refresh_inventory()
            return
        if self.trantorian.player_state.level == 8:
            self.transition_to(HelpTeamMatesState)
            return
        if self.trantorian.player_state.level == 1:
            tile = self.trantorian.player_state.vision.get_tile_index_of("linemate")
            if not tile:
                self.transition_to(GatherState)
                return
            self.trantorian.move_to_tile(tile)
            self.transition_to(EvolveState)
            return
        if self.trantorian.have_layed != 1:
            self.transition_to(ReproduceState)
            return
        if self.trantorian.status == "FOLLOWER":
            self.transition_to(FollowerState)
            self.trantorian.refresh_inventory()
            return
        if self.trantorian.status == "LEADER":
            self.transition_to(LeaderState)
            self.trantorian.refresh_inventory()
            return

    def transition_to(self, state_class):
        if not isinstance(self.state, state_class):
            self.state = state_class(self.trantorian, self)

    def execute_state(self):
        self.state.execute()

    def process_server_events(self):
        while True:
            if (
                hasattr(self.trantorian.connection, "event_queue")
                and len(self.trantorian.connection.event_queue) == 0
            ):
                break
            if len(self.trantorian.connection.event_queue) < 0:
                continue
            event = self.trantorian.connection.get_next_event()
            if event is None:
                break
            if event.event_type == "dead":
                self.trantorian.logger.warning("dead")
                self.trantorian.connection.running = False
                return False  # Ajoute le False ici pour signaler la mort !

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

            # Ajoute ce retour True vital pour que la condition de survie soit validée
            return True
