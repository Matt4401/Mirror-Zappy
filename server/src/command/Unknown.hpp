/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Unknown command
*/

#pragma once

#include "ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

class Unknown : public ACommand {
  public:
    Unknown();
    ~Unknown() override = default;
    Unknown(const Unknown& other) = delete;
    Unknown& operator=(const Unknown& other) = delete;
    Unknown(Unknown&& other) = delete;
    Unknown& operator=(Unknown&& other) = delete;

    bool start(game::World& world, game::Player& player) override;
    void execute(game::World& world, game::Player& player) override;

  private:
    static constexpr int kTimeLimit = 0;
};
}  // namespace zappy::server::command
