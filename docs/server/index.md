# Zappy Server Documentation

## Overview
Zappy is a network game where several teams confront each other on a geographically flat tile map containing resources. The ultimate goal for each team is to have at least 6 players reach the maximum elevation (Level 8) through a specific incantation ritual.

The server acts as the authoritative game engine, managing the world state, enforcing rules, and broadcasting events to both AI clients (players) and GUI clients (spectators).

## Technical Constraints
This server is built to adhere to strict performance and architectural constraints defined by the project specifications:

* **Language:** C++20.
* **Execution Model:** Single-process and single-threaded. 
* **Network Multiplexing:** Non-blocking asynchronous I/O using `poll()`.
* **Active Waiting:** Strictly forbidden. The server must only unlock if an event occurs on a socket or a game tick is ready for execution.
* **Time Management:** All game actions are bound to a strict frequency (`freq`), where 1 time unit equals `1.0 / freq` seconds.

## Documentation Structure
This documentation breaks down the server into its three primary layers:

1.  **[Architecture](architecture.md):** The high-level design patterns.
2.  **[Network Layer](network.md):** TCP socket management and message extraction.
3.  **[Core Orchestrator](core.md):** Time synchronization and command translation.
4.  **[Game Engine](game_engine.md):** Map data, player states, and command execution.
