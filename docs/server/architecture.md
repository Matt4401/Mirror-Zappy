# High-Level Architecture

The Zappy server is built using a hybrid of the **Reactor Pattern** for networking and a **Discrete-Time Game Loop** for simulation. 

Because the server must remain strictly single-threaded without dropping network inputs or lagging the game clock, the architecture aggressively separates the unpredictable nature of TCP networking from the deterministic nature of the game rules.

## Core Design Philosophy

1.  **Isolation of Concerns:** The network layer (`SessionManager`) knows nothing about the game (players, tiles, food). The game engine (`World`) knows nothing about file descriptors or sockets.
2.  **The Middleman:** The `Core` acts as the orchestrator. It maps network IDs to player IDs, translates raw strings into executable commands, and dictates when time moves forward.
3.  **Asynchronous Command Queuing:** Client commands are instantly parsed and pushed to a player's internal queue. They are only executed when the required number of game ticks has elapsed.

## System Diagram

The following diagram illustrates the structural relationships between the major components.

```mermaid
classDiagram
    %% The Orchestrator
    class Core {
        <<Orchestrator>>
        -SessionManager _sessionManager
        -World _world
        -CommandFactory _commandFactory
        -map~int,ClientState~ _clientStates
        -map~int,size_t~ _clientToPlayer
        +loop() void
        +processNetworkEvents() void
        +processGameTick() void
    }

    %% The Network Layer
    class SessionManager {
        <<Network Layer>>
        -ServerSocket _serverSocket
        -map~int,ClientSocket~ _clients
        -queue~NetworkEvent~ _eventQueue
        +pollNetwork(int timeout)
        +tryPopMessage()
        +sendMessage(clientId, msg)
    }

    %% The Translator
    class CommandFactory {
        <<Translator>>
        +createCommand(string_view) unique_ptr~ICommand~
    }

    %% The Game Engine
    class World {
        <<Game Engine>>
        -map~size_t,Player~ _playerList
        -vector~string~ _guiEvents
        +update()
        +pushCommandToPlayer(playerId, ICommand)
    }

    class Player {
        -queue~ICommand~ _commands
        -ICommand _currentCommand
        -int _cmdTick
        +update(World)
        +tryStartNextCommand(World)
    }

    class ICommand {
        <<interface>>
        +start(World, Player) bool
        +execute(World, Player)
    }

    %% Relationships
    Core "1" *-- "1" SessionManager : Polls and Flushes
    Core "1" *-- "1" World : Ticks time
    Core "1" *-- "1" CommandFactory : Translates text
    SessionManager "1" *-- "1" ServerSocket : Binds and listens
    SessionManager "1" *-- "many" ClientSocket : Reads/Writes
    World "1" *-- "many" Player : Manages
    Player "1" *-- "many" ICommand : Queues
