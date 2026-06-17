class SendCommand:
    def __init__(self, connection):
        self.connection = connection

    def forward(self):
        self.connection.send_connection("Forward")

    def right(self):
        self.connection.send_connection("Right")

    def left(self):
        self.connection.send_connection("Left")

    def look(self):
        self.connection.send_connection("Look")

    def inventory(self):
        self.connection.send_connection("Inventory")

    def broadcast(self, txt):
        self.connection.send_connection("Broadcast <" + txt + ">")

    def available_team_slots(self):
        self.connection.send_connection("Connect_nbr")

    def fork(self):
        self.connection.send_connection("Fork")

    def eject(self):
        self.connection.send_connection("Eject")

    def take_object(self, obj):
        self.connection.send_connection("Take <" + obj + ">")

    def set_object_down(self, obj):
        self.connection.send_connection("Set <" + obj + ">")

    def start_incantation(self):
        self.connection.send_connection("Incantation")
