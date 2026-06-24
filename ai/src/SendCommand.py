class SendCommand:
    def __init__(self, connection):
        self.connection = connection

    def forward(self):
        return self.connection.send_command("Forward")

    def right(self):
        return self.connection.send_command("Right")

    def left(self):
        return self.connection.send_command("Left")

    def look(self):
        return self.connection.send_command("Look")

    def inventory(self):
        return self.connection.send_command("Inventory")

    def broadcast(self, txt):
        return self.connection.send_command("Broadcast " + txt)

    def available_team_slots(self):
        return self.connection.send_command("Connect_nbr")

    def fork(self):
        return self.connection.send_command("Fork")

    def eject(self):
        return self.connection.send_command("Eject")

    def take_object(self, obj):
        return self.connection.send_command("Take " + obj)

    def set_object_down(self, obj):
        return self.connection.send_command("Set " + obj)

    def start_incantation(self):
        return self.connection.send_command("Incantation")
