/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** gui commands interface
*/

#pragma once

#include <string>

namespace zappy::server {
class Core;
}  // namespace zappy::server

namespace zappy::server::guiCommand {

class IGuiCommand {
  public:
    IGuiCommand() = default;
    IGuiCommand(const IGuiCommand& other) = delete;
    IGuiCommand& operator=(const IGuiCommand& other) = delete;
    IGuiCommand(IGuiCommand&& other) = delete;
    IGuiCommand& operator=(IGuiCommand&& other) = delete;
    virtual ~IGuiCommand() = default;

    virtual std::string execute(Core& core) = 0;
};

}  // namespace zappy::server::guiCommand
