# ⚡ Zappy - AI Network Game

Zappy is a network game where several teams confront each other on a tile map containing resources. The game board represents the entirety of this world's surface like a flat world map, meaning if a player exits to the right, they come back through the left, and similarly for up/down directions. The winning team is the first one where at least 6 players reach the maximum elevation level (level 8).

This repository contains three distinct software components working together over TCP sockets:

1. **`zappy_server`**: A server written in C++ that generates the inhabitants' world. It runs as a single process and single thread, utilizing `poll` to handle socket multiplexing.
2. **`zappy_gui`**: A graphical client written in C++ with Raylib used to watch what happens in the world. It authenticates itself by sending `GRAPHIC` to the server.
3. **`zappy_ai`**: An autonomous client in Python that drives an inhabitant drone through orders.

---

## 🛠️ Prerequisites & Dependencies

Ensure you have the following installed on your system before compiling:

### System Packages (Linux/Ubuntu)

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libconfig++-dev libraylib-dev
```

### Python Environment (For AI)

```bash
pip install torch ruff pre-commit
```

---

## 🏗️ Compilation

The project Makefile contains rules to compile the eponymous binaries:

```bash
# Compile all binaries (Server, GUI, and AI launcher)
make

# Compile specific components
make zappy_server
make zappy_gui
make zappy_ai
```

To clean build artifacts:

```bash
make fclean
```

---

## 🚀 Usage Guide

### 1. Starting the Server

```bash
./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq
```

**Options description:**

* `-p port`: Port number.
* `-x width`: Width of the world.
* `-y height`: Height of the world.
* `-n name1 name2...`: Name of the teams.
* `-c clientsNb`: Number of authorized clients per team.
* `-f freq`: Reciprocal of the time unit for execution of actions.

### 2. Launching the Graphical Client

```bash
./zappy_gui -p port -h machine
```

**Options description:**

* `-p port`: Port number.
* `-h machine`: Hostname of the server.

### 3. Deploying AI Agents

```bash
./zappy_ai -p port -n name -h machine
```

**Options description:**

* `-p port`: Port number.
* `-n name`: Name of the team.
* `-h machine`: Name of the machine (localhost by default).

---

<!-- Upcoming ## 🧪 Quick Sandbox Execution

For local testing, a quick-launch script can spin up a local instance:

```bash
chmod +x run.sh
./run.sh
``` -->

## 🤝 Contributing

Please refer to [CONTRIBUTING.md](docs/CONTRIBUTING.md) for guidelines regarding our strict Conventional Commits policy, pre-commit configuration hooks, and code review workflows.
