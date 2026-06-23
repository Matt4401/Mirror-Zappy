/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Look
*/

#pragma once
#include "ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {
class Look : public ACommand {
  public:
    Look();
    ~Look() override = default;
    Look(const Look& other) = delete;
    Look& operator=(const Look& other) = delete;
    Look(Look&& other) = delete;
    Look& operator=(Look&& other) = delete;

    void execute(game::World& world, game::Player& player) override;

  private:
    static constexpr int kTimeLimit = 7;
};
}  // namespace zappy::server::command
