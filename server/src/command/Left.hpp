/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Left
*/

#pragma once
#include "ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {
class Left : public ACommand {
  public:
    Left();
    ~Left() override = default;
    Left(const Left& other) = delete;
    Left& operator=(const Left& other) = delete;
    Left(Left&& other) = delete;
    Left& operator=(Left&& other) = delete;

    void execute(game::World& world, game::Player& player) override;

  private:
    static constexpr int kTimeLimit = 7;
};
}  // namespace zappy::server::command
