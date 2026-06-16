/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** msz
*/

#pragma once

#include <string>

#include "guiCommand/IGuiCommand.hpp"

namespace zappy::server::guiCommand {

class Bct : public IGuiCommand {
  public:
    Bct(int x, int y);
    Bct() = delete;
    Bct(const Bct& other) = delete;
    Bct& operator=(const Bct& other) = delete;
    Bct(Bct&& other) = delete;
    Bct& operator=(Bct&& other) = delete;
    ~Bct() override = default;

    std::string execute(Core& core) override;

  private:
    int _x;
    int _y;
};
}  // namespace zappy::server::guiCommand
