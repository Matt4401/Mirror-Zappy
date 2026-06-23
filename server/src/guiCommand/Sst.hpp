/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** SST
*/

#pragma once

#include "guiCommand/IGuiCommand.hpp"
#include "protocol/Commands.hpp"

namespace zappy::server::guiCommand {

class Sst : public IGuiCommand {
  public:
    Sst(shared::protocol::client::Sst cmd);
    Sst() = delete;
    Sst(const Sst& other) = delete;
    Sst& operator=(const Sst& other) = delete;
    Sst(Sst&& other) = delete;
    Sst& operator=(Sst&& other) = delete;
    ~Sst() override = default;

    GuiResponse execute(Core& core) override;

  private:
    int _timeUnit;
};

}  // namespace zappy::server::guiCommand
