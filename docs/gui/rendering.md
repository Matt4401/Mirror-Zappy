# Rendering

The render layer owns the window, camera, world scene, map picking, first-person view, skybox, and HUD registration.

## Main Classes

| Class | Responsibility |
| --- | --- |
| `Render` | Main frame lifecycle, input modes, camera updates, draw order. |
| `Map` | Draws tiles, resources, teams, players, eggs, and handles 3D picking. |
| `GameModel` | Draws player and egg models, applies player animations. |
| `FirstPerson` | Locks the camera to a selected player's eyes. |
| `Skybox3D` | Draws and updates the sky/time environment. |
| `AssetManager` | Caches fonts and textures by ID. |

## Draw Order

`Render::renderFrame()`:

1. update runtime state;
2. begin drawing;
3. draw 2D background layer, currently empty;
4. draw 3D scene;
5. draw UI;
6. end drawing.

`Render::render3D()`:

```cpp
_camera.beginMode3D();
_skybox.draw();
_map.draw();
Camera::endMode3D();
```

## Map Rendering

`Map::draw()` renders:

1. visible tiles;
2. tile resource objects;
3. teams, players, and eggs;
4. hovered tile bounding box.

Resources are dispatched by name through `_itemDrawFunctions`. If you add a new resource object, update both:

* `TileManager::_resourceFactories`;
* `Map::_itemDrawFunctions` and model fields.

## Floating Island (Underworld)

To give the map a distinct look, a procedural floating island extends downwards from the map grid.
The island uses Perlin noise to generate a depth map, bounded by a radial mask to ensure it tapers to a point underneath.
The island rendering is highly optimized through volume culling: only exterior blocks are sent to the GPU.
The floating island can be toggled via `showFloatingIsland` in the settings, and fully respects the camera's render distance frustum culling.

## Picking

Picking only runs when `Render` is in UI mode and the UI is not hovered.

Flow:

1. `Render::update()` calls `_map.handleEvent()`.
2. `Map` builds a mouse ray from the camera.
3. If `settings.showTiles` is true, it tests visible tile bounding boxes for hover.
4. On left click, if `settings.showPlayers` is true, it tests visible player bounding boxes first.
5. If a player is hit, dispatch `events::PlayerClicked`.
6. Otherwise, if a tile is hovered, dispatch `events::TileClicked`.

Player bounding boxes come from `game::Player::boundingBox()`. Hidden entities are completely skipped for interaction.

## Camera Modes

### Free Camera

Default camera mode uses Raylib `CAMERA_FREE`. Movement speed is now scaled by delta-time so that it remains consistent regardless of framerate. The user can configure base movement speed in settings. `Render` clamps the camera to a padded area around the map and prevents going below the ground.

### UI Mode

`Left Alt` toggles UI mode. When UI mode is active:

* cursor is enabled;
* camera free movement is disabled;
* clicking the 3D scene selects players/tiles if the UI is not hovered.

### Pause Mode

`Escape` toggles the pause menu. While the pause menu is visible:

* cursor is enabled;
* `_updateMode` is `PauseMenuOnly`;
* camera/world picking does not update.

The pause menu resume callback must restore `_updateMode = UpdateMode::All`.

### First-Person Mode

The player inspector can dispatch `PlayerFirstPersonRequested`. `FirstPerson` then:

* stores the selected player ID;
* moves camera to `player.position() + EyeHeight`;
* points camera in the player's rendered direction;
* updates every frame so the camera follows movement and rotation.

`Escape` exits first-person mode.

### Follow Mode

When the user selects a player by clicking on them in the 3D scene, the camera automatically tracks that player's position. While in follow mode:

* The camera's position and target update automatically to match the player's movements.
* The user can still rotate and zoom the camera freely around the tracked player using the mouse.
* Free movement via WASD is disabled to ensure the camera stays anchored to the player.
* Follow mode stops when the Player Inspector UI is closed, or when the followed player dies.

## Procedural Generation (Names & Skins)

To give each Trantorian a unique and recognizable identity, the GUI procedurally generates attributes when a new player connects (`pnw`):

* **NameGenerator**: Automatically assigns a unique, lore-friendly name to the player. The names are dynamically generated, making it easier to track individual players in the UI and Event Log without relying purely on their IDs.
* **SkinGenerator**: Assigns a random, visually distinct skin (texture ID) to the player's 3D model. This ensures a diverse population of Trantorians on the map.

These generated attributes are persistently tied to the player's ID for the duration of their life and are used across the rendering and UI systems.

## Asset Loading

Static models are generally owned by render classes as wrapper objects. UI textures and dynamic player skins should go through `AssetManager` so they are cached and reused.

Common asset locations:

* `assets/models/environment/`
* `assets/models/resources/`
* `assets/images/ui/`
* `assets/fonts/`
* `assets/audio/`
