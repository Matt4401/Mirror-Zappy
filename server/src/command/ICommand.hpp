/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Command interface
*/

#pragma once

#include "game/World.hpp"

class Player;

namespace zappy::server::command {

class ICommand {
  public:
    ICommand() = default;
    ICommand(const ICommand& other) = delete;
    ICommand& operator=(const ICommand& other) = delete;
    ICommand(ICommand&& other) = delete;
    ICommand& operator=(ICommand&& other) = delete;
    virtual ~ICommand() = default;

    [[nodiscard]] virtual int requiredTicks() const = 0;

    virtual bool start(game::World& world, Player& player) = 0;
    virtual void execute(game::World& world, Player& player) = 0;
};

}  // namespace zappy::server::command
