/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** bct
*/

#pragma once

#include <cstdint>
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
    std::uint32_t _x;
    std::uint32_t _y;
};
}  // namespace zappy::server::guiCommand
