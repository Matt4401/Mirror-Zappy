/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Sgt
*/

#pragma once

#include <string>

#include "guiCommand/IGuiCommand.hpp"

namespace zappy::server::guiCommand {

class Sgt : public IGuiCommand {
  public:
    Sgt() = default;
    Sgt(const Sgt& other) = delete;
    Sgt& operator=(const Sgt& other) = delete;
    Sgt(Sgt&& other) = delete;
    Sgt& operator=(Sgt&& other) = delete;
    ~Sgt() override = default;

    std::string execute(Core& core) override;
};
}  // namespace zappy::server::guiCommand
