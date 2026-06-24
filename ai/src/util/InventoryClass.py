class Inventory:
    def __init__(self):
        self.linemate = 0
        self.deraumere = 0
        self.sibur = 0
        self.mendiane = 0
        self.phiras = 0
        self.thystame = 0
        self.food = 10

    def update_inventory(
        self, linemate, deraumere, sibur, mendiane, phiras, thystame, food
    ):
        self.linemate = linemate
        self.deraumere = deraumere
        self.sibur = sibur
        self.mendiane = mendiane
        self.phiras = phiras
        self.thystame = thystame
        self.food = food

    def get_food(self):
        return self.food
