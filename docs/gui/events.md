# Event System

The GUI uses `events::EventDispatcher` as a typed message bus. It is the glue between network commands, world managers, rendering interactions, UI panels, and socket send requests.

## Typed Subscriptions

Subscribers register a callback for one concrete type:

```cpp
auto token = dispatcher.subscribe<shared::protocol::server::Pnw>(
    [](const shared::protocol::server::Pnw& command) {
        // update local state
    });
```

`subscribe<T>()` returns an `EventToken`. Store the token and unsubscribe when the owner is destroyed.

## Dispatching

Any type can be dispatched:

```cpp
dispatcher.dispatch(events::PlayerClicked{...});
dispatcher.dispatch(shared::protocol::server::Bct{...});
```

For network commands, `Client` dispatches a `shared::protocol::ServerCommand` variant. `EventDispatcher::dispatch(const ServerCommand&)` unwraps the variant and forwards the concrete command to `dispatch<T>()`.

## GUI Events

GUI-only events live in `gui/src/events/GuiEvents.hpp`.

| Event | Purpose |
| --- | --- |
| `PlayerClicked` | Map picked a player; player inspector should open/update. |
| `TileClicked` | Map picked a tile; tile inspector should open/update. |
| `SendCommand` | UI wants to send a text command to the server. |
| `PlayerNameChanged` | UI changed a generated player name. |
| `PlayerFirstPersonRequested` | Player inspector requested first-person camera. |
| `RequestCyclePlayer` | Inspector wants to select previous/next player. |
| `TimeOfDayChanged` | World control changed skybox time mode. |

## Unsubscribe Rules

Always unsubscribe in destructors for long-lived UI and scene objects.

Good pattern:

```cpp
if (_token != 0) {
    _dispatcher.get().unsubscribe<EventType>(_token);
}
```

`WorldManager` uses a slightly different pattern: it stores unsubscribe lambdas in `_unsubscribers`, then runs them in its destructor. This keeps all server command subscriptions centralized.

## Where Events Should Be Used

Use events when systems are peers and should not own each other:

* map click -> inspector panel;
* UI command -> network client;
* first-person request -> render camera controller.

Use direct method calls when one object clearly owns another:

* `WorldManager` routing server commands to `TileManager` and `PlayerManager`;
* `Render` configuring callbacks on `PauseMenu`.

