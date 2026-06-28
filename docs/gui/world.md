# World State

The world state in the GUI client represents the "source of truth" according to the server. It manages the map, tiles, players, eggs, and teams, maintaining an abstract game model decoupled from rendering.

## WorldManager

`WorldManager` is the central owner of game state. It listens to network events (via `EventDispatcher`) and delegates them to specific managers, or stores global properties (like time unit, map size).
It owns:

- `TileManager`: Handles the grid of `Tile3D` objects and their resources.
- `PlayerManager`: Tracks connected players, their inventories, levels, and team affiliations.
- `EggManager`: Tracks eggs laid on the map.

## TileManager

The `TileManager` constructs the physical grid of `Tile3D` when the server sends `msz` (Map Size).
When the server sends `bct` (Block Content), the `TileManager` updates the resource counts (food, linemate, etc.) on that specific tile.
The `TileManager` also manages the instantiation of 3D models representing these resources through factories.

## PlayerManager

The `PlayerManager` handles the lifecycle of Trantorians:

- `pnw` (Player New): Creates a `Player`, assigning a random name and skin via procedural generation.
- `ppo` (Player Position): Updates the position and orientation of a player, triggering interpolation to make movement smooth.
- `pdi` (Player Death): Marks the player as dead, which will trigger the death animation and eventual removal.

## Teams

Teams are simple containers for team names and colors. The `WorldManager` parses `tna` (Team Name) events to populate the list of valid teams. Each team gets a distinct color which is used to tint the skins of players in that team.
