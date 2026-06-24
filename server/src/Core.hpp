/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Core
*/

#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>

#include "command/CommandFactory.hpp"
#include "game/World.hpp"
#include "network/ISessionManager.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server {

class Core {
  public:
    explicit Core(std::span<char*> args);
    explicit Core(parser::ServerConfig config);
    ~Core() = default;

    Core(const Core& other) = delete;
    Core& operator=(const Core& other) = delete;
    Core(Core&& other) = delete;
    Core& operator=(Core&& other) = delete;

    int run();
    void stop();

    [[nodiscard]] const game::World& world() const { return *_world; }
    [[nodiscard]] parser::ServerConfig& config() { return _config; }

  private:
    enum class ClientState : std::uint8_t { WAITING_TEAM_SELECTION, IN_GAME, GUI };

    void setup();

    static void formatReceivedString(std::string& str);

    void processNetworkEvents();
    void processGameTick(std::chrono::steady_clock::time_point& nextTickTarget);

    void handleNewClient(int clientId);
    void handleClientMessage(int clientId, std::string_view message);
    void handleClientDisconnection(int clientId);
    void handleHandshake(int clientId, std::string_view teamName);
    void handleInGameMessage(int clientId, std::string_view message);
    void handleGuiMessage(int clientId, std::string_view message);

    void loop();

    void flushPlayerResponses();
    void flushGuiResponses();
    void sendGuiInitialState(int clientId);

    void sendGuiNewPlayerData(int clientId, std::size_t playerId);

    parser::ServerConfig _config;
    std::unique_ptr<shared::network::ISessionManager> _sessionManager;
    std::unique_ptr<game::World> _world;
    command::CommandFactory _commandFactory;

    int _timeUnit{0};
    bool _isRunning{true};

    std::span<char*> _args;
    std::unordered_map<int, ClientState> _clientStates;
    std::unordered_map<int, std::size_t> _clientToPlayer;
};

}  // namespace zappy::server
