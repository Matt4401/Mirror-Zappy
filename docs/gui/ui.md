# UI System

The GUI UI is built from reusable components and menu panels. It is drawn after the 3D scene and receives events in reverse component order so top-level components get priority.

## UIManager

`UIManager` owns a list of `std::shared_ptr<IUIComponent>`.

Every frame it:

1. updates visible components;
2. sends events to visible components in reverse order;
3. draws visible components;
4. reports whether any visible component is hovered.

`Render` uses `UIManager::isHovered()` to avoid sending map clicks through UI panels.

## Component Interface

All UI components implement `IUIComponent`:

* `draw()`
* `update()`
* `handleEvent()`
* `setPosition()`
* `setSize()`
* `isVisible()`
* `setVisible()`
* `isHovered()`

Reusable components include:

| Component | Role |
| --- | --- |
| `UIButton` | Clickable button, global click sound handler. |
| `UISlider` | Drag slider with `cancelDrag()` support. |
| `UIText` | Text rendering. |
| `UIImage` | Image rendering. |
| `UIPanel` | Basic panel background. |
| `UIGamePanel` | Movable/resizable game panel base. |
| `UIDropdown` | Selectable dropdown. |
| `UITextbox` | Editable text input. |
| `UICompass` | Camera direction display. |

## GameHUD

`GameHUD` constructs the main UI:

* `UIGridManager`
* `UICompass`
* `PauseMenu`
* `PlayerInspectorUI`
* `TileInspectorUI`
* `WorldControlUI`
* `EventLogUI`
* `GlobalStatsUI`

It registers only the root components in `UIManager`:

* compass;
* grid manager;
* pause menu.

The grid manager owns the normal HUD panels.

## UI Grid

`UIGridManager` uses a 64 x 36 virtual grid. Panels are registered with grid coordinates and dimensions. Config mode lets the user move/resize panels.

Current default layout:

| Panel | Grid Position |
| --- | --- |
| Player inspector | `(52, 16, 10, 15)` |
| Tile inspector | `(52, 3, 10, 12)` |
| World control | centered near bottom |
| Event log | `(2, 16, 12, 15)` |
| Global stats | `(2, 2, 12, 13)` |

`Escape` exits config mode.

## Pause Menu

`PauseMenu` is a full-screen overlay with:

* resume button;
* settings panel;
* UI configuration entry;
* save and quit action.

Settings currently expose several tabs:

* **Audio**: Master, Music, and SFX volumes, plus a Mute option.
* **Video**: Display mode (Windowed, Borderless, Fullscreen), resolution, FPS limit, FOV, camera speed, and render distance.
* **Controls**: Fully rebindable actions for movement, sprint, pause, toggle UI, and camera reset, with support for mouse keys.
* **Visibility**: Toggles for showing UI, players, minerals, food, and tiles.

Settings are saved and loaded persistently via `SettingsManager` which writes atomically to `config.ini`. When the menu is hidden, slider drag state is cancelled so a drag cannot continue after reopening.

`PauseMenu` does not directly modify `Render` state except through callbacks. `Render` installs:

* `setOnResume()`
* `setOnExit()`
* `setOnUIConfig()`

## Inspectors

### AInspectorUI

Base class for inspector panels. It provides:

* close button;
* shared inventory rendering;
* server frequency subscription;
* token storage for derived subscriptions;
* `onSendCommand` callback for protocol requests.

### PlayerInspectorUI

Opened by `events::PlayerClicked`.

Responsibilities:

* display selected player ID, team, position, orientation, level, health/food, and inventory;
* request `ppo`/`pin` synchronization when needed;
* update on `Ppo`, `Pin`, and `Plv`;
* request first-person camera mode;
* allow generated player name updates through `PlayerNameChanged`.

### TileInspectorUI

Opened by `events::TileClicked`.

Responsibilities:

* display tile coordinates and inventory;
* request `bct x y` synchronization;
* react to `bct`, `pic`, and `pie`.

## Button Sounds

All `UIButton` instances share a static click sound handler. `GameHUD` installs it with:

```cpp
UIButton::setClickSoundHandler([audio] {
    audio->playSound("button_click");
});
```

The handler is cleared in `GameHUD` destructor.
