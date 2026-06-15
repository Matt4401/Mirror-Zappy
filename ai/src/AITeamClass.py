from src.AIConnection import AIConnection
import threading

class AITeam:
    def __init__(self, trantorian, port, host, team_name):
        self.trantorian = trantorian
        self.thread = threading.Lock()
        self.answer_list = []
        self.data_lock = threading.Lock()
        self.connection = AIConnection(host, port, team_name, self.data_lock, self.answer_list)
        self.reader_thread = threading.Thread(target=self.connection.run_reader, daemon=True)

    def threat_start(self):
        self.reader_thread.start()

    def run(self):
        thread_start()
