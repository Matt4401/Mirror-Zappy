# Audio System

The Zappy GUI features a fully spatialized 3D audio system and UI sound effects, managed by the `AudioManager`.

## AudioManager

The `AudioManager` wraps Raylib's audio device initialization and provides an interface for playing music and sound effects.

### Spatial Sounds

The audio system supports 3D positional audio. `AudioManager::setListener(position, target)` is called every frame by the `Render` loop, updating the listener's perspective based on the current camera position.
When an event occurs in the world (e.g., player broadcasting `pbc`, egg hatching `eht`), a sound can be played at that exact 3D coordinate using `AudioManager::playSpatialSound`.

### UI Sounds

UI components (like `UIButton`) share a static click handler that calls `AudioManager::playSound("button_click")`. These are 2D sounds that bypass the spatial system, playing directly in stereo.

### Pitch Scaling

To make repetitive sounds less annoying and more dynamic (e.g., footsteps, UI clicks), the `AudioManager` can apply random pitch shifting to sound instances when they are played.
