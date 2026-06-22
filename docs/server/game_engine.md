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
```

By storing IDs (like `std::size_t`) rather than full objects, the engine ensures cache-friendly memory access when players Look, Take, or Drop items. The actual `Player` objects are stored securely in a master hash map (`_playerList`) managed by the `World`.

## The Player State Machine

Players are not active threads; they are passive state machines updated by the `World`'s tick cycle.

### The Command Queue
When the network layer receives a command, it is pushed to the player's internal queue.
According to Zappy protocol rules, **a player can only queue a maximum of 10 commands.** If a client sends an 11th command before the first 10 finish, the engine silently ignores and drops it. This prevents malicious clients from flooding the game's execution queue.

### The Tick Cycle (`_cmdTick`)
Each player tracks their current action using `_cmdTick`. 
1. When `World::update()` is called, it calls `Player::update()`, which decrements `_cmdTick`.
2. If `_cmdTick` reaches `0`, the current command completes. `ICommand::execute()` is triggered, applying the final effects (like moving the player or updating their inventory) and generating an `ok\n` response.

## Zero-Tick Failures & Instant Execution

To eliminate "active waiting" and guarantee instant GUI feedback, the engine utilizes a `tryStartNextCommand()` mechanism.

When a player is idle and a command arrives, or when an executing command finishes, `tryStartNextCommand()` attempts to pull the next action from the queue.

* **Valid Commands:** The command's `start()` method is called. This instantly starts any necessary GUI events (like `pfk` for Fork or `pic` for Incantation) into the network buffer and sets `_cmdTick` to the required duration (e.g., 42 ticks).
* **Invalid Commands:** If the client sends an unknown command, or tries to execute an impossible action, `start()` returns `false`. The engine instantly generates a `ko\n` response and consumes **0 ticks**, looping immediately to the next command in the queue. 

This architecture guarantees that failing commands do not penalize the player's execution time, maintaining perfect alignment with the official Zappy specifications.
