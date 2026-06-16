class PlayerState:
    def __init__(self, team_name):
        self.level = 0
        self.team_name = team_name
        self.position = {10, 10}  # TODO randomise here
        self.direction = {1}  # # TODO randomise here
        self.vision = (
            None  # TODO we have to create a norm for the vision in an other issu
        )
        self.inventory = []  # TODO we have to create a struct or eaven a class for inventory in an other issue
        self.food_remaining = 10

    def food(self):
        return self.food_remaining

    def update_food(self, new_food):
        self.food_remaining += new_food

    def position(self):
        return self.position

    def update_position(self, new_position):
        self.position = new_position

    def direction(self):
        return self.direction

    def update_direction(self, new_direction):
        self.direction = new_direction

    def vision(self):
        return self.vision

    def update_vision(self, new_vision):
        self.vision = new_vision

    def inventory(self):
        return self.inventory

    def update_inventory(self, new_inventory):
        self.inventory = new_inventory

    def update_vision(self, vision):
        self.vision = vision

    def vision(self):
        return self.vision

    def upgrade_level(self):
        self.level += 1

    def level(self):
        return self.level
