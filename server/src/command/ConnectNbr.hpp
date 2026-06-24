/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ConnectNbr command
*/

#pragma once

#include "command/ACommand.hpp"
#include "command/ICommand.hpp"

namespace zappy::server::command {

class ConnectNbr : public ACommand {
  public:
    ConnectNbr();
    ~ConnectNbr() override = default;
    ConnectNbr(const ConnectNbr& other) = delete;
    ConnectNbr& operator=(const ConnectNbr& other) = delete;
    ConnectNbr(ConnectNbr&& other) = delete;
    ConnectNbr& operator=(ConnectNbr&& other) = delete;

    bool start(game::World& world, game::Player& player) override;
    void execute(game::World& world, game::Player& player) override;

  private:
    static constexpr int kTimeLimit = 0;
};

}  // namespace zappy::server::command
