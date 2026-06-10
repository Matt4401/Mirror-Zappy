/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Client
*/

#pragma once

#include "parsing/strategy/GUIStrategy.hpp"
#include <memory>
#include "network/IClientSocket.hpp"

namespace zappy::gui::network {
class Client {
  public:
    static constexpr auto DefaultTeamName = "GRAPHIC";

    explicit Client(const parser::parsing::GuiConfig& config);
    ~Client() = default;
    Client(const Client& other) = delete;
    Client& operator=(const Client& other) = delete;
    Client(Client&& other) = delete;
    Client& operator=(Client&& other) = delete;

    [[nodiscard]] shared::network::IClientSocket& socket() { return *_socket; }

  private:
    std::unique_ptr<shared::network::IClientSocket> _socket;
};
}  // namespace zappy::gui::network
