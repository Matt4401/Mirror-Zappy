class SendCommand:
    def __init__(self, connection):
        self.connection = connection

    def send_forward(self):
        self.connection.send_connection("Forward")

    def send_right(self):
        self.connection.send_connection("Right")

    def send_left(self):
        self.connection.send_connection("Left")

    def send_look(self):
        self.connection.send_connection("Look")

    def send_inventory(self):
        self.connection.send_connection("Inventory")

    def send_broadcast(self, txt):
        self.connection.send_connection("Broadcast <" + txt + ">")

    def send_available_team_slots(self):
        self.connection.send_connection("Connect_nbr")

    def send_fork(self):
        self.connection.send_connection("Fork")

    def send_eject(self):
        self.connection.send_connection("Eject")

    def send_take_object(self, obj):
        self.connection.send_connection("Take <" + obj + ">")

    def send_set_object_down(self, obj):
        self.connection.send_connection("Set <" + obj + ">")

    def send_start_incantation(self):
        self.connection.send_connection("Incantation")
