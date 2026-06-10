/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Right
*/

#pragma once
#include "ACommand.hpp"

namespace zappy::server::command {
class Right : public ACommand {
  public:
    Right();
    ~Right() override = default;
    Right(const Right& other) = delete;
    Right& operator=(const Right& other) = delete;
    Right(Right&& other) = delete;
    Right& operator=(Right&& other) = delete;

    void execute(game::World& world, game::Player& player) override;

  private:
    static constexpr int kTimeLimit = 7;
};
}  // namespace zappy::server::command
