/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Core
*/

#pragma once

#include <memory>
#include <span>

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
    void setup();
    void loop() const;

    std::span<char*> _args;
    parser::ServerConfig _config;
    int _timeUnit{0};
    std::unique_ptr<shared::network::ISessionManager> _sessionManager;
    std::unique_ptr<game::World> _world;
    command::CommandFactory _commandFactory;
    bool _isRunning{true};
};

}  // namespace zappy::server
