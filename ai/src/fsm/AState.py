from abc import ABC, abstractmethod


class AState(ABC):
    def __init__(self, trantorian):
        self.trantorian = trantorian

    @abstractmethod
    def execute(self):
        pass
