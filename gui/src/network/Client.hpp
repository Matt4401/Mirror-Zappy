/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Client
*/

#pragma once

#include "network/ClientSocket.hpp"
#include "parsing/strategy/GUIStrategy.hpp"

namespace zappy::gui::network {
class Client {
  public:
    static constexpr auto DefaultTeamName = "GRAPHIC";

    explicit Client(const shared::parsing::GuiConfig & config);
    ~Client() = default;
    Client(const Client& other) = delete;
    Client& operator=(const Client& other) = delete;
    Client(Client&& other) = delete;
    Client& operator=(Client&& other) = delete;

    [[nodiscard]] const shared::network::ClientSocket& socket() const { return _socket; }

  private:
    shared::network::ClientSocket _socket;
};
}  // namespace zappy::gui::network
