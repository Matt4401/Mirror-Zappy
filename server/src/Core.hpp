/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Core
*/

#pragma once

#include <memory>

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
    util::Config _config;
    int _timeUnit;

    std::unique_ptr<shared::network::ISessionManager> _sessionManager;
    game::World _world;
    command::CommandFactory _commandFactory;
    bool _isRunning{true};
};

}  // namespace zappy::server
