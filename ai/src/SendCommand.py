from AIConnection import AIConnection


class SendCommand:
    def __init__(self, connection):
        self.connection = connection

    def send_forward(self):
        connection.send_connection("Forward")

    def send_right(self):
        connection.send_connection("Right")

    def send_left(self):
        connection.send_connection("Left")

    def send_look(self):
        connection.send_connection("Look")

    def send_inventory(self):
        connection.send_connection("Inventory")

    def send_broadcast(self, txt):
        connection.send_connection("Broadcast <" + txt + ">")

    def send_available_team_slots(self):
        connection.send_connection("Connect_nbr")

    def send_fork(self):
        connection.send_connection("Fork")

    def send_eject(self):
        connection.send_connection("Eject")

    def send_take_object(self, obj):
        connection.send_connection("Take <" + obj + ">")

    def send_set_object_down(self, obj):
        connection.send_connection("Set <" + obj + ">")

    def send_start_incantation(self):
        connection.send_connection("Incantation")
