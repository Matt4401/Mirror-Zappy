from abc import ABC, abstractmethod


class AState(ABC):
    def __init__(self, trantorian, fsm):
        self.trantorian = trantorian
        self.fsm = fsm

    @abstractmethod
    def execute(self):
        pass
