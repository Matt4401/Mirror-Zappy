/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Fork command
*/

#pragma once

#include "command/ACommand.hpp"
#include "command/ICommand.hpp"

namespace zappy::server::command {

class Fork : public ACommand {
  public:
    Fork();
    ~Fork() override = default;
    Fork(const Fork& other) = delete;
    Fork& operator=(const Fork& other) = delete;
    Fork(Fork&& other) = delete;
    Fork& operator=(Fork&& other) = delete;

    bool start(game::World& world, game::Player& player) override;
    void execute(game::World& world, game::Player& player) override;

  private:
    static constexpr int kTimeLimit = 42;
};

}  // namespace zappy::server::command
