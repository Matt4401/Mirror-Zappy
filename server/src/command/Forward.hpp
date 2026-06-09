/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Forward command
*/

#pragma once

#include "command/ACommand.hpp"
#include "command/ICommand.hpp"

class Player;

namespace zappy::server::command {

class Forward : public ACommand {
  public:
    Forward();
    ~Forward() override = default;
    Forward(const Forward& other) = delete;
    Forward& operator=(const Forward& other) = delete;
    Forward(Forward&& other) = delete;
    Forward& operator=(Forward&& other) = delete;

    void execute(game::World& world, game::Player& player) override;
};

}  // namespace zappy::server::command
