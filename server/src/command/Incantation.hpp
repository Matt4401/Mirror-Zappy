/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Incantation
*/

#pragma once
#include <array>
#include <cstddef>
#include <cstdint>

#include "ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

class Incantation : public ACommand {
  public:
    Incantation();
    ~Incantation() override = default;
    Incantation(const Incantation& other) = delete;
    Incantation& operator=(const Incantation& other) = delete;
    Incantation(Incantation&& other) = delete;
    Incantation& operator=(Incantation&& other) = delete;

    bool start(game::World& world, game::Player& player) override;
    void execute(game::World& world, game::Player& player) override;

  private:
    static constexpr int kTimeLimit = 300;
};
}  // namespace zappy::server::command
