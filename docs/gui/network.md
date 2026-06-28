    # Network and Protocol

The GUI network layer is intentionally thin. It connects to the server, identifies as the graphical client, parses incoming messages, and dispatches typed commands.

## Client Connection

Class: `gui/src/network/Client.*`

Connection sequence:

1. Create a `ClientSocket` from GUI config (`machine`, `port`).
2. Wait up to 5 seconds for `WELCOME`.
3. Send `GRAPHIC\n`.
4. Subscribe to `events::SendCommand` so UI panels can ask the server for data.

The GUI uses:

```cpp
static constexpr auto DefaultTeamName = "GRAPHIC\n";
```

This is not a normal team. It puts the connection into server GUI mode.

## Receiving Messages

`Client::update()` repeatedly calls `tryPopMessage()`:

```cpp
while (auto msg = _socket->tryPopMessage()) {
    auto serverCmd = shared::protocol::Parser::parseServerCommand(*msg);
    _dispatcher.get().dispatch(serverCmd);
}
```

The shared protocol parser returns `shared::protocol::ServerCommand`, a variant of all possible server-to-GUI commands. The dispatcher then routes the concrete command type to subscribers.

## Sending GUI Requests

UI panels never write to the socket directly. They dispatch:

```cpp
events::SendCommand{payload}
```

`Client` listens to `SendCommand` and sends the payload through the socket.

Examples:

| Request | Why it is sent |
| --- | --- |
| `ppo #id\n` | Refresh a player position/orientation. |
| `pin #id\n` | Refresh a player inventory. |
| `bct x y\n` | Refresh a tile inventory. |
| `sst f\n` | Ask server to change the frequency. |

## Adding a GUI Protocol Command

1. Add or verify the command structure in `shared/include/protocol/Commands.hpp`.
2. Add parser/emitter support in the shared protocol layer if missing.
3. Subscribe to the command in `WorldManager` or a UI panel.
4. Update local state in the correct manager.
5. Add focused tests under `tests/src/gui/` when behavior is stateful.

The network layer should not contain world-specific logic.

