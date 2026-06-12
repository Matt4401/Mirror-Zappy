from src.AIConnection import *


class AITeam:
    def __init__(self, trantorian, port, host, team_name):
        self.trantorian = trantorian
        self.AIConnection = AIConnection(host, port, team_name)

    def run(self):
        pass
