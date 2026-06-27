from ..AState import AState


class ReproduceState(AState):
    def execute(self):
        self.trantorian.send_command.fork()
        self.trantorian.have_layed += 1
