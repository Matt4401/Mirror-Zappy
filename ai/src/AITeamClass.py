from src.AIConnection import AIConnection
import threading


class AITeam:
    def __init__(self, trantorian, port, host, team_name):
        self.trantorian = trantorian
        self.thread = threading.Lock()
        self.answer_list = []
        self.data_lock = threading.Lock()
        self.team_name = team_name
        self.connection = AIConnection(
            host, port, team_name, self.data_lock, self.answer_list
        )
        self.player_state(team_name, )
        self.reader_thread = threading.Thread(
            target=self.connection.run_reader, daemon=True
        )

    def thread_start(self):
        self.reader_thread.start()

    def run(self):
        self.thread_start()
