/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Core
*/

#pragma once

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
    ~Core() = default;

    Core(const Core& other) = delete;
    Core& operator=(const Core& other) = delete;
    Core(Core&& other) = delete;
    Core& operator=(Core&& other) = delete;

    int run();
    void stop();

  private:
    enum class ClientState : std::uint8_t { WAITING_TEAM_SELECTION, IN_GAME };

    static void formatReceivedString(std::string& str);

    void handleNewClient(int clientId);
    void handleClientMessage(int clientId, std::string_view message);
    void handleClientDisconnection(int clientId);

    void setup();
    void loop();

    void flushPlayerResponses();

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
