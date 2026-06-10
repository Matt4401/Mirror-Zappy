/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Command interface
*/

#pragma once
#include <cstddef>

namespace zappy::server::game {
class Player;
class World;
}  // namespace zappy::server::game

namespace zappy::server::command {

class ICommand {
  public:
    ICommand() = default;
    ICommand(const ICommand& other) = delete;
    ICommand& operator=(const ICommand& other) = delete;
    ICommand(ICommand&& other) = delete;
    ICommand& operator=(ICommand&& other) = delete;
    virtual ~ICommand() = default;

    [[nodiscard]] virtual std::size_t requiredTicks() const = 0;

    virtual bool start(game::World& world, game::Player& player) = 0;
    virtual void execute(game::World& world, game::Player& player) = 0;
};

}  // namespace zappy::server::command
