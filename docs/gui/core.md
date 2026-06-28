# Core Runtime

`gui::Core` is the GUI orchestrator. It parses command line arguments, creates the network client, creates the renderer, and runs the main loop.

## Startup

Entry point:

```cpp
int main(int ac, char** av) {
    const auto args = std::span{av, static_cast<std::size_t>(ac)};
    return zappy::gui::Core(args).run();
}
```

Startup sequence:

1. `Core::run()` calls `setup()`.
2. `setup()` parses GUI arguments using `parser::GUIStrategy`.
3. `Core` creates `network::Client`.
4. `Core` creates `graphics::Render`.
5. `run()` enters `loop()`.

The important long-lived objects are:

| Object | Owner | Role |
| --- | --- | --- |
| `EventDispatcher` | `Core` | Shared message bus for server commands and GUI events. |
| `AudioManager` | `Core` | Owns Raylib audio device, music, active sounds. |
| `network::Client` | `Core` | Receives server messages and sends GUI commands. |
| `graphics::Render` | `Core` | Owns window, camera, scene, HUD, and render loop. |

## Main Loop

`Core::loop()` runs while `Render::isRunning()` is true:

```cpp
while (_render && _render->isRunning()) {
    if (_client) {
        _client->update();
    }
    _render->renderFrame();
    _audioManager.update();
}
```

The order matters:

1. **Network first:** all pending server commands are dispatched before drawing.
2. **Render second:** the frame uses the latest world state.
3. **Audio last:** music streaming and active sound positions are updated once per frame.

## Error Handling

`Core::run()` catches project exceptions and standard exceptions. Parser usage requests return success; other errors are printed and return failure.

## Render Frame Lifecycle

`Render::renderFrame()` executes:

1. `update()`
2. `Window::beginDrawing()`
3. `render2D()`
4. `render3D()`
5. `_uiManager.draw()`
6. `Window::endDrawing()`

`render2D()` is currently empty. The main visual work is in `render3D()` and UI drawing.

## Input Modes

`Render` tracks two concepts:

* `_uiMode`: the cursor is available for interacting with map/UI.
* `_updateMode`: either `All` or `PauseMenuOnly`.

Important controls:

| Input | Behavior |
| --- | --- |
| `Escape` | Exit first-person view, exit UI config mode, or toggle pause menu. |
| `Left Alt` | Toggle UI cursor mode when not in first-person/config mode. |
| Left click | In UI mode, pick players or tiles if the UI is not hovered. |

When the pause menu is visible, `Render` uses `PauseMenuOnly` so camera movement stops. The resume callback must restore `UpdateMode::All`.

