# The Game Engine

The game engine is the deterministic heart of the Zappy server. It is entirely contained within the `World` and `Player` classes and is responsible for enforcing all protocol rules, managing resources, and tracking the progression of time.

## The World Map
Trantor is represented as a 2D spherical grid (`_tiles`). If an entity moves off the right edge, it wraps around to the left edge (modulo arithmetic).

Each `Tile` is a lightweight structure holding vectors of IDs and resource counts:
```cpp
struct Tile {
    std::vector<std::size_t> players;
    std::vector<std::size_t> eggs;
    std::array<std::size_t, static_cast<uint8_t>(ItemType::COUNT)> resources;
};
