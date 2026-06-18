import re


class ParseCommand:
    def __init__(self, inventory):
        self.inventory = inventory

    def parse_inventory(self, data):
        items = re.findall(r"(\w+)\s+(\d+)", data)
        data_dict = {name: int(value) for name, value in items}
        required = [
            "linemate",
            "deraumere",
            "sibur",
            "mendiane",
            "phiras",
            "thystame",
            "food",
        ]
        missing = [k for k in required if k not in data_dict]
        if missing:
            raise ValueError(f"Missing inventory fields: {', '.join(missing)}")
        linemate = data_dict["linemate"]
        deraumere = data_dict["deraumere"]
        sibur = data_dict["sibur"]
        mendiane = data_dict["mendiane"]
        phiras = data_dict["phiras"]
        thystame = data_dict["thystame"]
        food = data_dict["food"]
        self.inventory.update_inventory(
            linemate, deraumere, sibur, mendiane, phiras, thystame, food
        )

    def parse_broadcast(self, data):
        pass

    def parse_look(self, data):
        pass

    def parse_eject(self, data):
        pass
