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
                clean_instruction = instruction.replace("need ", "").strip()
                stone_list = clean_instruction.split(" ")
                for stone in stone_list:
                    if self.trantorian.player_state.vision.get_tile_index_of(stone) == 0:
                        self.trantorian.take_object(stone)
                        #self.trantorian.refresh_inventory()
                if self.trantorian.traveling_stone is None:
                    stone = random.choice(stone_list)
                    tile_index = self.trantorian.player_state.vision.get_tile_index_of(
                        stone
                    )
                    if tile_index is not None:
                        self.trantorian.move_to_tile(tile_index)
                        res = self.trantorian.take_object(stone)
                        if res and res[0]:
                            val = getattr(
                                self.trantorian.player_state.inventory, stone, 0
                            )
                            setattr(
                                self.trantorian.player_state.inventory, stone, val + 1
                            )
                    return
                else:
                    if direction == 0:
                        self.trantorian.set_object_down(self.trantorian.traveling_stone)
                        return
                    self.trantorian.move_one_step_toward(direction)
                return
            if "join" in instruction:
                status, level, raw_instruction = instruction.split(" ")
                if level != self.trantorian.player_state.level:
                    return
                if direction == 0:
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