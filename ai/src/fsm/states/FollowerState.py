import random
from ..AState import AState


class FollowerState(AState):
    def execute(self):
        self.trantorian.logger.info("===========Entering Follower state===========")
        if not len(self.trantorian.received_broadcasts) > 0:
            self.trantorian.send_command.forward()
            return

        latest_broadcast = self.trantorian.received_broadcasts.pop(0)
        direction = latest_broadcast.get("direction")
        raw_message = latest_broadcast.get("msg")
        decoded = self.trantorian.broadcast_manager.read_broadcast(raw_message)
        if decoded:
            sender_id, instruction = decoded
            if "need" in instruction:
                clean_instruction = instruction.split("need ")[1].strip()
                stone_list = clean_instruction.split(" ")
                for stone in stone_list:
                    if (
                        self.trantorian.player_state.vision.get_tile_index_of(stone)
                        == 0
                    ):
                        res = self.trantorian.take_object(stone)
                        if res and res[0]:
                            val = getattr(
                                self.trantorian.player_state.inventory, stone, 0
                            )
                            setattr(
                                self.trantorian.player_state.inventory, stone, val + 1
                            )
                has_needed_stone = False
                for stone in stone_list:
                    if getattr(self.trantorian.player_state.inventory, stone, 0) > 0:
                        has_needed_stone = True
                        break
                if has_needed_stone:
                    if direction == 0:
                        for stone in [
                            "linemate",
                            "deraumere",
                            "sibur",
                            "mendiane",
                            "phiras",
                            "thystame",
                        ]:
                            count = getattr(
                                self.trantorian.player_state.inventory, stone, 0
                            )
                            for _ in range(count):
                                res = self.trantorian.set_object_down(stone)
                                if res and res[0]:
                                    val = getattr(
                                        self.trantorian.player_state.inventory, stone, 0
                                    )
                                    setattr(
                                        self.trantorian.player_state.inventory,
                                        stone,
                                        val - 1,
                                    )
                    else:
                        self.trantorian.move_one_step_toward(direction)
                else:
                    if self.trantorian.traveling_stone is None:
                        stone = random.choice(stone_list)
                        tile_index = (
                            self.trantorian.player_state.vision.get_tile_index_of(stone)
                        )
                        if tile_index is not None:
                            self.trantorian.move_to_tile(tile_index)
                            res = self.trantorian.take_object(stone)
                            if res and res[0]:
                                val = getattr(
                                    self.trantorian.player_state.inventory, stone, 0
                                )
                                setattr(
                                    self.trantorian.player_state.inventory,
                                    stone,
                                    val + 1,
                                )
                        return
                    self.trantorian.move_one_step_toward(direction)
                return
            if "join" in instruction:
                status, level, raw_instruction = instruction.split(" ")
                if int(level) != self.trantorian.player_state.level:
                    return
                if direction == 0:
                    if self.fsm.tick_manager._should_broadcast():
                        self.trantorian.logger.info("[Follower]: On leader emplacement")
                        msg = self.trantorian.broadcast_manager.create_message(
                            self.trantorian.status, "Here"
                        )
                        self.trantorian.send_command.broadcast(msg)
                else:
                    self.trantorian.logger.info(
                        f"[Follower]: To leader emplacement (direction {direction})"
                    )
                    self.trantorian.move_one_step_toward(direction)


# faire en sorte de gratter ressources sur le chemin -> fait
# rajouter if de est ce qu'on est concerné par le moove
