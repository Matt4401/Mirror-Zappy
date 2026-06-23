/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** msz
*/

#pragma once

#include "guiCommand/IGuiCommand.hpp"

namespace zappy::server::guiCommand {

class Msz : public IGuiCommand {
  public:
    Msz() = default;
    Msz(const Msz& other) = delete;
    Msz& operator=(const Msz& other) = delete;
    Msz(Msz&& other) = delete;
    Msz& operator=(Msz&& other) = delete;
    ~Msz() override = default;

    GuiResponse execute(Core& core) override;
};
}  // namespace zappy::server::guiCommand
