# Raylib Wrappers

The `gui/raylib/` directory contains an object-oriented C++23 wrapper around the Raylib C API.
This wrapper ensures RAII (Resource Acquisition Is Initialization) for Raylib resources, preventing memory leaks, and provides strong typing.

## Design Philosophy

- **RAII**: Resources like `Texture2D`, `Model`, `Sound`, and `Font` are wrapped in classes that call their respective `Unload*` function in the destructor. Move semantics are implemented, but copy semantics are deleted to prevent double-free errors.
- **Namespaces**: All wrappers belong to `zappy::gui::raylib::*`, with sub-namespaces mirroring Raylib modules (`rcore`, `rmath`, `rmodels`, `rtextures`, `rtext`, `raudio`).
- **Const Correctness**: Functions that don't modify the underlying Raylib structure are marked `const`.

## Common Wrappers

- **`rmath::Vector3` / `Vector2`**: Wraps Raylib vectors with operator overloading for vector math.
- **`rcore::Camera`**: Encapsulates `Camera3D` and manages projection, FOV, and orbital rotations.
- **`rmodels::Model`**: Loads and unloads 3D `.obj` or `.iqm` models, and handles drawing them at positions.
- **`rmodels::ModelAnimation`**: Manages skeletal animations.
- **`rtext::Font`**: Wraps TTF loading and text measurement.
- **`raudio::Sound` & `raudio::Music`**: RAII wrappers for audio streams and sound buffers.

By keeping Raylib strictly contained within these wrappers, the rest of the GUI application remains decoupled from the raw C API.
