/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Client
*/

#include "Client.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <utility>

#include "EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "exception/SocketError.hpp"
#include "protocol/Parser.hpp"
#include "shared/include/network/IClientSocket.hpp"
#include "socket/ClientSocket.hpp"
#include "strategy/GUIStrategy.hpp"

namespace zappy::gui::network {
Client::Client(const parser::GuiConfig& config, std::shared_ptr<events::EventDispatcher> dispatcher)
    : _socket{std::make_unique<::network::socket::ClientSocket>(config.machine, config.port)},
      _dispatcher{std::move(dispatcher)} {
    initNetwork();
}

Client::Client(std::unique_ptr<shared::network::IClientSocket> socket,
               std::shared_ptr<events::EventDispatcher> dispatcher)
    : _socket{std::move(socket)}, _dispatcher{std::move(dispatcher)} {
    initNetwork();
}

void Client::initNetwork() {
    std::optional<std::string> welcomeMsg;
    auto startTime = std::chrono::steady_clock::now();
    while (!(welcomeMsg = _socket->tryPopMessage())) {
        if (std::chrono::steady_clock::now() - startTime > std::chrono::seconds(5)) {
            throw ::network::exception::SocketError(std::string("timeout waiting for WELCOME from server"));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if (welcomeMsg != "WELCOME") {
        throw ::network::exception::SocketError(std::string("expected WELCOME from server, got: ") +
                                                welcomeMsg.value_or(""));
    }

    if (_socket->send(DefaultTeamName) == 0) {
        throw ::network::exception::SocketError(std::string("failed to send team name"));
    }

    if (_dispatcher) {
        _sendToken = _dispatcher->subscribe<events::SendCommand>([this](const events::SendCommand& cmd) {
            if (_socket->send(cmd.payload) == 0) {
                std::cerr << "Failed to send command to server: " << cmd.payload << std::endl;
            }
        });
    }
}

Client::~Client() {
    if (_dispatcher && _sendToken != 0) {
        _dispatcher->unsubscribe<events::SendCommand>(_sendToken);
    }
}

void Client::update() {
    while (auto msg = _socket->tryPopMessage()) {
        auto serverCmd = shared::protocol::Parser::parseServerCommand(*msg);
        if (_dispatcher) {
            _dispatcher->dispatch(serverCmd);
        }
    }
}

}  // namespace zappy::gui::network
