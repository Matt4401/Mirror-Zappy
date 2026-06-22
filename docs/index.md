# Zappy Documentation Hub

Welcome to the official technical documentation for the Zappy project.

Zappy is a distributed, network-based game set on the planet Trantor. In this project, players (driven by autonomous AI clients) explore a 2D tile-based world, collect resources, and perform incantation rituals to increase their elevation level. The server acts as the authoritative engine, managing the map state, broadcasting events to graphical clients, and enforcing the game rules.



## Project Components

This project is divided into four primary modules:

| Component | Description |
| :--- | :--- |
| **[Server](server/index.md)** | The core game engine and network multiplexer. Handles game logic, resource management, and command execution. |
| **[AI Client](ai/index.md)** | The autonomous agent that connects to the server to control a player (drone) through the Zappy protocol. |
| **[GUI Client](gui/index.md)** | The graphical visualization tool that authenticates as "GRAPHIC" to monitor the world in real-time. |
| **[Shared](network.md)** | Common utilities, network interfaces, exception handling, and protocol definitions shared across the project. |

## Quick Start

### Prerequisites
* **Language:** C++23
* **Tools:** `poll()` for socket multiplexing, CMake for build automation.

---

## Contributing
For guidelines on coding standards, commit messages, and documentation updates, please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) file.
