/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Plv
*/

#pragma once
#include "IGuiCommand.hpp"
#include "protocol/Commands.hpp"

namespace zappy::server::guiCommand {
class Plv : public IGuiCommand {
  public:
    explicit Plv(shared::protocol::client::Plv cmd);
    Plv() = delete;
    Plv(const Plv& other) = delete;
    Plv& operator=(const Plv& other) = delete;
    Plv(Plv&& other) = delete;
    Plv& operator=(Plv&& other) = delete;
    ~Plv() override = default;

    GuiResponse execute(Core& core) override;

  private:
    int _id;
};
}  // namespace zappy::server::guiCommand
