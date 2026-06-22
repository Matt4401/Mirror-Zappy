# Zappy - AI Network Game

Zappy is a network game where several teams confront each other on a tile map containing resources. The game board
represents the entirety of this world's surface like a flat world map, meaning if a player exits to the right, they come
back through the left, and similarly for up/down directions. The winning team is the first one where at least 6 players
reach the maximum elevation level (level 8).

This repository contains three distinct software components working together over TCP sockets:

1. **`zappy_server`**: A server written in C++ that generates the inhabitants' world. It runs as a single process and
   single thread, utilizing `poll` to handle socket multiplexing.
2. **`zappy_gui`**: A graphical client written in C++ with Raylib used to watch what happens in the world. It
   authenticates itself by sending `GRAPHIC` to the server.
3. **`zappy_ai`**: An autonomous client in Python that drives an inhabitant drone through orders.

---

## Prerequisites & Dependencies

Ensure you have the following installed on your system before compiling:

### System Packages (Linux/Ubuntu)

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libconfig++-dev libraylib-dev
```

*(Arch Linux users: `sudo pacman -S base-devel cmake libconfig raylib`)*

### Python Environment (For AI)

```bash
python3 -m pip install torch ruff pre-commit
```

---

## Compilation

This project relies on **CMake** and **Makefile** for its build system. It is highly recommended to use an out-of-source
build by generating files into a `build/` directory.

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

## Usage Guide

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

## Testing

The project includes a robust, data-driven functional test suite built with Python and `pytest`.

The test suite performs **differential testing**: it automatically boots both the official reference binary and your
custom `zappy_server`, executes a series of TCP network scenarios (defined in `functional_tests/scenarios.yaml`), and
ensures your server's responses match the official protocol perfectly.

To launch the test suite, use the dedicated CMake target. This command will automatically rebuild the server if needed,
create a safe Python virtual environment, install dependencies, and run the tests:

```bash
cmake --build build --target tests_functional
```

*Note: The test script automatically manages its own dependencies inside `functional_tests/.venv` to avoid polluting
your global Python environment.*

---

## Contributors

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/EstebanPeroz">
        <img src="https://github.com/EstebanPeroz.png?size=100" width="100px;" alt="EstebanPeroz"/>
        <br />
        <sub><b>EstebanPeroz</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/TomGatin">
        <img src="https://github.com/TomGatin.png?size=100" width="100px;" alt="TomGatin"/>
        <br />
        <sub><b>TomGatin</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/N0AH-sve">
        <img src="https://github.com/N0AH-sve.png?size=100" width="100px;" alt="N0AH-sve"/>
        <br />
        <sub><b>N0AH-sve</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/Matt4401">
        <img src="https://github.com/Matt4401.png?size=100" width="100px;" alt="Matt4401"/>
        <br />
        <sub><b>Matt4401</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/eliottraguin">
        <img src="https://github.com/eliottraguin.png?size=100" width="100px;" alt="eliottraguin"/>
        <br />
        <sub><b>eliottraguin</b></sub>
      </a>
    </td>
  </tr>
</table>

---

## Contributing

Please refer to [CONTRIBUTING.md](docs/CONTRIBUTING.md) for guidelines regarding our strict Conventional Commits policy,
pre-commit configuration hooks, and code review workflows.

## Documentation

The full technical documentation is available online at **[INSERT_LINK_WHEN_AVAILABLE]**.

Architecture diagrams and source files are also located in the `docs/` folder. To serve the documentation locally:

```bash
python3 -m venv docs/.venv
source docs/.venv/bin/activate
pip install -r docs/requirements.txt
mkdocs serve
```
