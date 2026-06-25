import re


class ParseCommand:
    def __init__(self, inventory):
        self.inventory = inventory

    def parse_inventory(self, data):
        clean_data = data.replace("[", "").replace("]", "").strip()
        clean_data = clean_data.replace(",", " ")
        items = re.findall(r"(\w+)\s+(\d+)", clean_data)
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
            raise ValueError(
                f"Missing inventory fields: {', '.join(missing)}. Received raw: '{data}'"
            )
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
        clean_data = data.strip("[]\n ")
        raw_tiles = clean_data.split(",")
        parsed_tiles = []
        for tile in raw_tiles:
            tile = tile.strip()
            if not tile:
                parsed_tiles.append([])
            else:
                objects_on_tile = tile.split()
                parsed_tiles.append(objects_on_tile)

        return parsed_tiles

    def parse_eject(self, data):
        pass
