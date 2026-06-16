from src.InventoryClass import Inventory


class ParseCommand:
    def __init__(self, inventory):
        self.inventory = inventory

    def parse_inventory(self, data):
        items = re.findall(r"(\w+)\s+(\d+)", data)
        data_dict = {name: int(value) for name, value in items}
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
