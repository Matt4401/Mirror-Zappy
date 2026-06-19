/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Eject
*/

#pragma once
#include "ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {
class Eject : public ACommand {
  public:
    Eject();
    ~Eject() override = default;
    Eject(const Eject& other) = delete;
    Eject& operator=(const Eject& other) = delete;
    Eject(Eject&& other) = delete;
    Eject& operator=(Eject&& other) = delete;

    bool start(game::World& world, game::Player& player) override;
    void execute(game::World& world, game::Player& player) override;

  private:
    static constexpr int kTimeLimit = 7;
};
}  // namespace zappy::server::command
