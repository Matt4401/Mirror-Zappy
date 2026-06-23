/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** bct
*/

#pragma once

#include "guiCommand/IGuiCommand.hpp"
#include "protocol/Commands.hpp"

namespace zappy::server::guiCommand {

class Bct : public IGuiCommand {
  public:
    explicit Bct(shared::protocol::client::Bct cmd);
    Bct() = delete;
    Bct(const Bct& other) = delete;
    Bct& operator=(const Bct& other) = delete;
    Bct(Bct&& other) = delete;
    Bct& operator=(Bct&& other) = delete;
    ~Bct() override = default;

    GuiResponse execute(Core& core) override;

  private:
    int _x;
    int _y;
};
}  // namespace zappy::server::guiCommand
