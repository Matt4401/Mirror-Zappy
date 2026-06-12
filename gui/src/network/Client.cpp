/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Client
*/

#include "Client.hpp"

#include <iostream>
#include <memory>
#include <variant>

#include "cli/Parser.hpp"
#include "exception/SocketError.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Parser.hpp"
#include "socket/ClientSocket.hpp"

namespace zappy::gui::network {
Client::Client(const cli::Parser::Config& config)
    : _socket{std::make_unique<::network::socket::ClientSocket>(config.machine, config.port)} {
    if (_socket->send(DefaultTeamName) == 0) {
        throw ::network::exception::SocketError("failed to send team name");
    }
}

void Client::update() {
    auto msg = _socket->tryPopMessage();
    if (msg) {
        shared::protocol::Parser const parser;
        auto serverCmd = parser.parseServerCommand(*msg);

        if (std::holds_alternative<shared::protocol::server::Msz>(
                serverCmd)) {  // TODO : instead of using holds_alternative + get, we should use std::visit with an
                               // overloaded lambda to avoid multiple parsing of the same command
            auto mszData = std::get<shared::protocol::server::Msz>(serverCmd);
            std::cout << "GUI received map size from server: " << mszData.width << "x" << mszData.height << std::endl;
        } else if (std::holds_alternative<shared::protocol::UnknownCommand>(serverCmd)) {
            std::cout << "GUI received unknown command or bad syntax." << std::endl;
        } else {
            std::cout << "GUI received another valid command." << std::endl;
        }
    }
}

}  // namespace zappy::gui::network
