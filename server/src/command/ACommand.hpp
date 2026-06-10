/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ACommand
*/

#pragma once

#include "ICommand.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

class ACommand : public ICommand {
  public:
    explicit ACommand(int requiredTicks) : _requiredTicks{requiredTicks} {}
    ~ACommand() override = default;

    ACommand(const ACommand& other) = delete;
    ACommand& operator=(const ACommand& other) = delete;
    ACommand(ACommand&& other) = delete;
    ACommand& operator=(ACommand&& other) = delete;

    [[nodiscard]] int requiredTicks() const override { return _requiredTicks; }

    [[nodiscard]] bool start(game::World& /*world*/, Player& /*player*/) override { return true; }

  protected:
    void setRequiredTicks(int ticks) { _requiredTicks = ticks; }

  private:
    int _requiredTicks;
};

}  // namespace zappy::server::command
