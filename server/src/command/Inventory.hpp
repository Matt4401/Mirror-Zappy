/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Inventory
*/

#pragma once

#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {
class Inventory : public ACommand {
  public:
    Inventory();
    ~Inventory() override = default;
    Inventory(const Inventory& other) = delete;
    Inventory& operator=(const Inventory& other) = delete;
    Inventory(Inventory&& other) = delete;
    Inventory& operator=(Inventory&& other) = delete;

    void execute(game::World& world, game::Player& player) override;

  private:
    static constexpr int kTimeLimit = 1;
};
}  // namespace zappy::server::command
