/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Ppo
*/

#pragma once

#include <cstddef>

#include "guiCommand/IGuiCommand.hpp"
#include "protocol/Commands.hpp"

namespace zappy::server::guiCommand {

class Ppo : public IGuiCommand {
  public:
    Ppo(shared::protocol::client::Ppo cmd);
    Ppo(const Ppo& other) = delete;
    Ppo& operator=(const Ppo& other) = delete;
    Ppo(Ppo&& other) = delete;
    Ppo& operator=(Ppo&& other) = delete;
    ~Ppo() override = default;

    GuiResponse execute(Core& core) override;

  private:
    std::size_t _id;
};
}  // namespace zappy::server::guiCommand
