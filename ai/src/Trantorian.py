from src.Connection import Connection
from src.PlayerState import PlayerState
from src.SendCommand import SendCommand
import threading
import math


class Trantorian:
    def __init__(self, port, host, team_name):
        self.thread = threading.Lock()
        self.answer_list = []
        self.data_lock = threading.Lock()
        self.team_name = team_name
        self.connection = Connection(host, port, team_name)
        self.player_state = PlayerState(team_name)
        self.send_command = SendCommand(self.connection)

    def move_to_tile(self, index):
        if index == 0:
            return
        n = int(math.sqrt(index))
        start_of_level = n * n
        dx = index - (start_of_level + n)
        for _ in range(n):
            self.send_command.forward()
        if dx < 0:
            for _ in range(abs(dx)):
                self.send_command.left()
        elif dx > 0:
            for _ in range(dx):
                self.send_command.right()
