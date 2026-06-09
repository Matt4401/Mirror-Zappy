/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ACommand
*/

#pragma once

#include "ICommand.hpp"

namespace zappy::server::command {

class ACommand : public ICommand {
  public:
    explicit ACommand(const int requiredTicks) : _requiredTicks{requiredTicks} {}
    ~ACommand() override = default;

    ACommand(const ACommand& other) = delete;
    ACommand& operator=(const ACommand& other) = delete;
    ACommand(ACommand&& other) = delete;
    ACommand& operator=(ACommand&& other) = delete;

    [[nodiscard]] std::size_t requiredTicks() const override { return _requiredTicks; }

    [[nodiscard]] bool start(game::World& /*world*/, game::Player& /*player*/) override { return true; }

  protected:
    void setRequiredTicks(const int ticks) { _requiredTicks = ticks; }

  private:
    int _requiredTicks;
};

}  // namespace zappy::server::command
