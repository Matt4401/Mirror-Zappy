/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Client
*/

#include "Client.hpp"

#include <memory>

#include "exception/SocketError.hpp"
#include "socket/ClientSocket.hpp"
#include "strategy/GUIStrategy.hpp"

namespace zappy::gui::network {
Client::Client(const parser::GuiConfig& config)
    : _socket{std::make_unique<::network::socket::ClientSocket>(config.machine, config.port)} {
    if (_socket->send(DefaultTeamName) == 0) {
        throw ::network::exception::SocketError("failed to send team name");
    }
}
}  // namespace zappy::gui::network
