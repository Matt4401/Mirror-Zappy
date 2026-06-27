from ..AState import AState


class ReproduceState(AState):
    def execute(self):
        id = self.trantorian.send_command.fork()
        self.trantorian.wait_for_response(id)
        self.trantorian.have_layed += 1
