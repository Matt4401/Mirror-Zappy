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

struct GuiResponse {
    std::string message;
    bool isArgumentError{false};
    bool isCommandError{false};
    bool sendToEveryone{false};
};

class IGuiCommand {
  public:
    IGuiCommand() = default;
    IGuiCommand(const IGuiCommand& other) = delete;
    IGuiCommand& operator=(const IGuiCommand& other) = delete;
    IGuiCommand(IGuiCommand&& other) = delete;
    IGuiCommand& operator=(IGuiCommand&& other) = delete;
    virtual ~IGuiCommand() = default;

    virtual GuiResponse execute(Core& core) = 0;
};

}  // namespace zappy::server::guiCommand
