/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Client
*/

#include "Client.hpp"

#include "exception/SocketError.hpp"
#include "util/DataStructures.hpp"

namespace zappy::gui::network {
Client::Client(const shared::parsing::GuiConfig& config) : _socket{config.machine, config.port} {
    if (_socket.send(DefaultTeamName) == -1) {
        throw shared::exception::SocketError("Failed to send GRAPHIC command");
    }
}
}  // namespace zappy::gui::network
