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
#include <string_view>
#include <unordered_map>

#include "command/CommandFactory.hpp"
#include "game/World.hpp"
#include "network/ISessionManager.hpp"
#include "util/DataStructures.hpp"

namespace zappy::server {

class Core {
  public:
    explicit Core(util::Config config);
    ~Core() = default;

    Core(const Core& other) = delete;
    Core& operator=(const Core& other) = delete;
    Core(Core&& other) = delete;
    Core& operator=(Core&& other) = delete;

    void run();
    void stop();

  private:
    enum class ClientState : std::uint8_t { WAITING_TEAM_SELECTION, IN_GAME };

    void handleNewClient(int clientId);
    void handleClientMessage(int clientId, std::string_view message);
    void handleClientDisconnection(int clientId);

    util::Config _config;
    int _timeUnit;

    std::unique_ptr<shared::network::ISessionManager> _sessionManager;
    game::World _world;
    command::CommandFactory _commandFactory;
    bool _isRunning{true};

    std::unordered_map<int, ClientState> _clientStates;
    std::unordered_map<int, std::size_t> _clientToPlayer;
};

}  // namespace zappy::server
