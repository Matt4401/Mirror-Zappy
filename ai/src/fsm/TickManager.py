from .Constant import LOOK_INTERVAL
from .Constant import INVENTORY_INTERVAL
from .Constant import BROADCAST_INTERVAL
from .Constant import PRIME_NUMBER_COMBINATION
from .Constant import DISTRIBUTION_VALUE


class TickManager:
    def __init__(self, agent_id: int):
        self.tick = 0
        self.current_tick = 0  # alias explicite utilisé dans FSM.py
        self.broadcast_offset = (
                                        agent_id * PRIME_NUMBER_COMBINATION
                                ) % DISTRIBUTION_VALUE
        self._last_broadcast_tick = 0  # FIX: pour ticks_since_last_broadcast()

    def tick_update(self) -> list[str]:
        cmds = []
        self.tick += 1
        self.current_tick = self.tick

        if self._should_inventory():
            cmds.append("Inventory")
        if self._should_look():
            cmds.append("Look")
        if self._should_broadcast():
            cmds.append(None)
            self._last_broadcast_tick = self.tick

        return cmds

    def _should_inventory(self) -> bool:
        return self.tick % INVENTORY_INTERVAL == 0

    def _should_look(self) -> bool:
        return self.tick % LOOK_INTERVAL == 0

    def _should_broadcast(self) -> bool:
        return (self.tick + self.broadcast_offset) % BROADCAST_INTERVAL == 0

    # FIX: méthode manquante appelée dans LeaderState.py
    def ticks_since_last_broadcast(self) -> int:
        return self.tick - self._last_broadcast_tick

    def mark_broadcast_sent(self):
        """Appeler manuellement après un broadcast hors cycle automatique."""
        self._last_broadcast_tick = self.tick