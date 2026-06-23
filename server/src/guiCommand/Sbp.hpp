/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Sbp
*/

#pragma once

#include "guiCommand/IGuiCommand.hpp"

namespace zappy::server::guiCommand {

class Sbp : public IGuiCommand {
  public:
    Sbp() = default;
    Sbp(const Sbp& other) = delete;
    Sbp& operator=(const Sbp& other) = delete;
    Sbp(Sbp&& other) = delete;
    Sbp& operator=(Sbp&& other) = delete;
    ~Sbp() override = default;

    GuiResponse execute(Core& /*core*/) override;
};
}  // namespace zappy::server::guiCommand
