/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Core
*/

#pragma once

#include "game/World.hpp"
#include "network/SessionManager.hpp"
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
    network::SessionManager _sessionManager;
    game::World _world;
    bool _isRunning{true};
};

}  // namespace zappy::server
