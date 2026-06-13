/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Client
*/

#include "Client.hpp"

#include <memory>
#include <utility>

#include "EventDispatcher.hpp"
#include "exception/SocketError.hpp"
#include "protocol/Parser.hpp"
#include "socket/ClientSocket.hpp"
#include "strategy/GUIStrategy.hpp"

namespace zappy::gui::network {
Client::Client(const parser::GuiConfig& config, std::shared_ptr<events::EventDispatcher> dispatcher)
    : _socket{std::make_unique<::network::socket::ClientSocket>(config.machine, config.port)},
      _dispatcher{std::move(dispatcher)} {
    if (_socket->send(DefaultTeamName) == 0) {
        throw ::network::exception::SocketError("failed to send team name");
    }
}

void Client::update() {
    auto msg = _socket->tryPopMessage();
    if (msg) {
        auto serverCmd = shared::protocol::Parser::parseServerCommand(*msg);
        _dispatcher->dispatch(serverCmd);
    }
}

}  // namespace zappy::gui::network
