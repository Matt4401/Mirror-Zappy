/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** mct
*/

#pragma once

#include "guiCommand/IGuiCommand.hpp"

namespace zappy::server::guiCommand {

class Mct : public IGuiCommand {
  public:
    Mct() = default;
    Mct(const Mct& other) = delete;
    Mct& operator=(const Mct& other) = delete;
    Mct(Mct&& other) = delete;
    Mct& operator=(Mct&& other) = delete;
    ~Mct() override = default;

    GuiResponse execute(Core& core) override;
};
}  // namespace zappy::server::guiCommand
