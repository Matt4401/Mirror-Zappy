import math


class Trantorian:
    def __init__(self, name, command_sender):
        self.name = name
        self.command_sender = command_sender

    def move_to_tile(self, index):
        if index == 0:
            return []
        n = int(math.sqrt(index))
        start_of_level = n * n
        dx = index - (start_of_level + n)
        commands = []
        if dx < 0:
            for _ in range(abs(dx)):
                self.command_sender.left()
        elif dx > 0:
            for _ in range(dx):
                self.command_sender.right()
        for _ in range(n):
            self.command_sender.forward()
