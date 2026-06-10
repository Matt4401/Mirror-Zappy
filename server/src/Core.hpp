/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Core
*/

#pragma once

#include "command/CommandFactory.hpp"
#include "game/World.hpp"
#include "network/SessionManager.hpp"

namespace zappy::server {

class Core {
  public:
    explicit Core(parser::parsing::ServerConfig config);
    ~Core() = default;

    Core(const Core& other) = delete;
    Core& operator=(const Core& other) = delete;
    Core(Core&& other) = delete;
    Core& operator=(Core&& other) = delete;

    void run();
    void stop();

  private:
    parser::parsing::ServerConfig _config;
    network::SessionManager _sessionManager;
    game::World _world;
    command::CommandFactory _commandFactory;
    bool _isRunning{true};
};

}  // namespace zappy::server
