import re
import time

from ..AState import AState
from ..Constant import (
    ELEVATION_REQUIREMENTS,
    BROADCAST_EVO_PROCESS,
    BROADCAST_DONE,
    INCANT_FOOD_ABORT,
    EVO_PROCESS_BROADCAST_PERIOD,
    FORK_WHILE_WAITING_FOOD,
    FORK_COOLDOWN_SECONDS,
)


class EvolveState(AState):
    def __init__(self, trantorian):
        super().__init__(trantorian)
        self.stones_placed = False
        self.wait_ticks = 0
        self.finished = False

    # utilitary functions
    def requirements(self):
        level = self.trantorian.player_state.level
        return level, ELEVATION_REQUIREMENTS.get(level + 1, {})

    def players_on_tile(self):
        tiles = self.trantorian.player_state.vision.get_tiles()
        return tiles[0].count("player") if tiles else 0

    def place_stones(self, req):
        if self.stones_placed:
            return
        for resource, amount in req.items():
            if resource == "player" or amount == 0:
                continue
            for _ in range(amount):
                self.trantorian.set_object_down(resource)
        self.stones_placed = True

    def reclaim_stones(self, req):
        if not self.stones_placed:
            return
        for resource, amount in req.items():
            if resource == "player" or amount == 0:
                continue
            for _ in range(amount):
                self.trantorian.take_object(resource)
        self.stones_placed = False
        self.trantorian.inventory()

    def broadcast(self, keyword, level):
        ps = self.trantorian.player_state
        self.trantorian.send_command.broadcast(
            f"{ps.team_name}|{keyword}:{level}:{self.trantorian.leader_id}"
        )

    def reset(self, reason: str):
        _, req = self.requirements()
        self.trantorian.logger.warning(f"[Evolve]: aborting evo process ({reason})")
        self.reclaim_stones(req)
        self.broadcast(BROADCAST_DONE, self.trantorian.player_state.level)
        self.finished = True

    def request(self, direction, level, leader):
        _, req = self.requirements()
        self.trantorian.logger.info(f"[Evolve]: yielding to lower leader {leader}")
        self.reclaim_stones(req)
        self.trantorian.player_state.set_incantation_target(direction, level, leader)
        self.finished = True

    def execute(self):
        ps = self.trantorian.player_state
        level, req = self.requirements()
        required_players = req.get("player", 1)

        if required_players <= 1:
            self.solo_incantation(req, level)
            return

        food = ps.inventory.get_food()
        if food <= INCANT_FOOD_ABORT:
            self.reset("food")
            return

        self.wait_ticks += 1
        if self.wait_ticks % EVO_PROCESS_BROADCAST_PERIOD == 0:
            self.broadcast(BROADCAST_EVO_PROCESS, level)

        self.trantorian.look()
        count = self.players_on_tile()
        self.trantorian.logger.info(
            f"[Evolve]: evo process lvl {level} -> {count}/{required_players} players"
        )

        if count >= required_players:
            self.place_stones(req)
            self.trantorian.look()
            if self.players_on_tile() >= required_players:
                self.launch(req, level)
                return
            return

       # can reproduce during wait others
        if (
                not self.stones_placed
                and food >= FORK_WHILE_WAITING_FOOD
                and self.trantorian.can_reproduce()
                and time.time() - self.trantorian.last_fork_time >= FORK_COOLDOWN_SECONDS
        ):
            self.trantorian.logger.info("[Evolve]: forking to grow population while waiting")
            self.trantorian.fork()
            self.trantorian.inventory()

    def solo_incantation(self, req, level):
        self.trantorian.logger.info("[Evolve]: solo incantation")
        self.place_stones(req)
        self.launch(req, level)
        self.finished = True

    def launch(self, req, level):
        self.trantorian.logger.info("[Evolve]: launching incantation")
        result = self.trantorian.start_incantation()
        self.handle_result(result, req, level)

    def handle_result(self, result, req, level):
        ps = self.trantorian.player_state
        if result is None:
            self.trantorian.logger.warning("[Evolve]: incantation got no answer")
            self.reset("no_answer")
            return
        success, response = result
        if success and "Current level" in response:
            match = re.search(r"Current level:\s*(\d+)", response)
            if match:
                new_level = int(match.group(1))
                if new_level > ps.level:
                    ps.level = new_level
                    ps.vision.set_level(new_level)
                    ps.vision.reset_on_turn()
            self.trantorian.logger.warning(f"[Evolve]: elevation success -> {ps.level}")
            self.stones_placed = False
            self.broadcast(BROADCAST_DONE, level)
            self.trantorian.inventory()
            self.finished = True
        else:
            self.trantorian.logger.warning(f"[Evolve]: elevation failed ({response})")
            self.reset("ko")
