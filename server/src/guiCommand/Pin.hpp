/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Pin
*/

#pragma once
#include "IGuiCommand.hpp"
#include "protocol/Commands.hpp"

namespace zappy::server::guiCommand {
class Pin : public IGuiCommand {
  public:
    explicit Pin(shared::protocol::client::Pin cmd);
    Pin() = delete;
    Pin(const Pin& other) = delete;
    Pin& operator=(const Pin& other) = delete;
    Pin(Pin&& other) = delete;
    Pin& operator=(Pin&& other) = delete;
    ~Pin() override = default;

    GuiResponse execute(Core& core) override;

  private:
    int _id;
};
}  // namespace zappy::server::guiCommand
