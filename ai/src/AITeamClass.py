from src.AIConnection import AIConnection
from src.PlayerState import PlayerState
from src.SendCommand import SendCommand
import threading


class AITeam:
    def __init__(self, trantorian, port, host, team_name):
        self.trantorian = trantorian
        self.thread = threading.Lock()
        self.answer_list = []
        self.data_lock = threading.Lock()
        self.team_name = team_name
        self.connection = AIConnection(host, port, team_name)
        self.player_state = PlayerState(team_name)
        self.send_command = SendCommand(self.connection)

    def run(self):
        pass
