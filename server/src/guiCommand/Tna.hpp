/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Tna
*/

#pragma once

#include "guiCommand/IGuiCommand.hpp"

namespace zappy::server::guiCommand {

class Tna : public IGuiCommand {
  public:
    Tna() = default;
    Tna(const Tna& other) = delete;
    Tna& operator=(const Tna& other) = delete;
    Tna(Tna&& other) = delete;
    Tna& operator=(Tna&& other) = delete;
    ~Tna() override = default;

    GuiResponse execute(Core& core) override;
};
}  // namespace zappy::server::guiCommand
