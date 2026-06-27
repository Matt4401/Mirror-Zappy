from ..AState import AState


class FollowerState(AState):
    def execute(self):
        pass
        # if broadcast message == pierre
        # dès qu'on en a une, on va dessus et pose
        # if broadcast message == join un certain level
        # on rejoint avec la direction jusqu'à arriver à  broadcast[direction == 0, "..."]
        # else
        # broadcast : on est là, la famille
