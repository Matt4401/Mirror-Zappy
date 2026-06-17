class VisionManager:
    def __init__(self):
        self.tiles = []
        self.current_level = 1

    def set_level(self, level):
        self.current_level = level

    def update_tiles(self, cleaned_new_tiles):
        self.tiles = cleaned_new_tiles

    def remove_resource_underfoot(self, resource_name):
        if self.tiles and resource_name in self.tiles[0]:
            self.tiles[0].remove(resource_name)

    def add_resource_underfoot(self, resource_name):
        if self.tiles:
            self.tiles[0].append(resource_name)

    def shift_on_forward(self):
        if not self.tiles:
            return
        total_tiles = (self.current_level + 1) ** 2
        new_tiles = [[] for _ in range(total_tiles)]

        for y in range(self.current_level):
            current_row_start = y ** 2
            next_row_start = (y + 1) ** 2
            row_length = (y * 2) + 1

            for i in range(row_length):
                old_index = next_row_start + i + 1
                new_index = current_row_start + i

                if old_index < len(self.tiles):
                    new_tiles[new_index] = self.tiles[old_index]
        self.tiles = new_tiles

    def get_tile_index_of(self, resource_name):
        for index, tile in enumerate(self.tiles):
            if resource_name in tile:
                return index
        return None

    def reset_on_turn(self):
        self.tiles = []
