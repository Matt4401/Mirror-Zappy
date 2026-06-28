# Zappy GUI Documentation

## Overview

The Zappy GUI is the spectator client of the project. It connects to the server as `GRAPHIC`, receives server protocol events, keeps a local read-only representation of the world, and renders that state as an interactive 3D scene with UI inspectors, audio feedback, and camera controls.

The GUI is not authoritative. It never decides game rules. Its job is to:

* connect to the server and parse GUI protocol commands;
* transform those commands into local scene state;
* render the map, resources, teams, players, eggs, animations, and skybox;
* expose interaction tools for developers and spectators;
* send GUI-side requests such as `ppo`, `pin`, `bct`, or `sst` when a panel needs refreshed data.

## Technical Constraints

* **Language:** C++23.
* **Rendering:** Raylib, wrapped by the local `gui/raylib/` layer.
* **Architecture:** Event-driven GUI state updates through `EventDispatcher`.
* **Networking:** The GUI uses the shared network socket layer and the shared protocol parser.
* **Runtime model:** One main loop owned by `gui::Core`.
* **Ownership:** Long-lived systems are owned by `Core` or `Render`; scene managers own world state; UI components are shared through `UIManager`.

## Documentation Structure

1. **[Architecture](architecture.md):** Main systems and data flow.
2. **[Core Runtime](core.md):** Startup, loop, network/render/audio ordering.
3. **[Network and Protocol](network.md):** GUI connection lifecycle and command dispatch.
4. **[Event System](events.md):** Typed subscriptions, GUI events, and unsubscribe rules.
5. **[World State](world.md):** `WorldManager`, `TileManager`, `PlayerManager`, and game models.
6. **[Rendering](rendering.md):** 3D scene, camera modes, picking, skybox, and assets.
7. **[UI System](ui.md):** HUD, panels, inspectors, pause menu, and layout grid.
8. **[Audio](audio.md):** Music, spatial sounds, pitch scaling, and UI sounds.
9. **[Raylib Wrappers](raylib_wrappers.md):** Local wrapper conventions.
10. **[Contributing](contributing.md):** How to add features safely.

## Quick Developer Map

| Goal | Start Here |
| --- | --- |
| Understand the GUI startup flow | `gui/src/Core.cpp`, [Core Runtime](core.md) |
| Add a server command handler | `WorldManager`, [World State](world.md) |
| Add or update rendering | `Map`, `GameModel`, [Rendering](rendering.md) |
| Add UI panel behavior | `GameHUD`, `UIManager`, [UI System](ui.md) |
| Add a sound | `AudioManager`, [Audio](audio.md) |
| Add a Raylib feature | `gui/raylib/`, [Raylib Wrappers](raylib_wrappers.md) |

