/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Zappy core
*/

#include "Core.hpp"

#include <exception>
#include <iostream>
#include <memory>
#include <span>
#include <string>
#include <utility>
#include <variant>

#include "Parser.hpp"
#include "SessionManager.hpp"
#include "exception/Exception.hpp"
#include "game/World.hpp"
#include "network/ISessionManager.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"
#include "protocol/Parser.hpp"
#include "util/DataStructures.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server {

Core::Core(const std::span<char*> args) : _args(args) {}

int Core::run() {
    try {
        setup();
        loop();
    } catch (const zappy::shared::exception::Exception& e) {
        if (std::string(e.what()) == zappy::parser::kUsageThrowMessage) {
            return zappy::parser::kExitSuccess;
        }
        std::cerr << "Error: " << e.what() << std::endl;
        return zappy::parser::kExitFailure;
    } catch (const std::exception& e) {
        std::cerr << "Unknown error: " << e.what() << std::endl;
        return zappy::parser::kExitFailure;
    }
    return zappy::parser::kExitSuccess;
}

void Core::setup() {
    auto serverStrategy = std::make_unique<zappy::parser::ServerStrategy>();
    zappy::parser::Parser<zappy::parser::ServerConfig> parser(std::move(serverStrategy));

    _config = parser.parse(static_cast<int>(_args.size()), _args.data());
    _sessionManager = std::make_unique<network::SessionManager>(_config.port);
    _world = std::make_unique<game::World>(_config);
    _timeUnit = static_cast<int>(1.0F / static_cast<float>(_config.freq) * 1000);
}

void Core::loop() const {
    shared::network::ISessionManager::NetworkEvent message{};

    while (_isRunning) {
        try {
            _sessionManager->pollNetwork(_timeUnit);
            while (_sessionManager->tryPopMessage(message)) {
                // TODO: Example of parsing the command (to be moved to a proper CommandHandler)
                auto clientCmd = shared::protocol::Parser::parseClientCommand(message.message);

                if (std::holds_alternative<shared::protocol::client::Msz>(
                        clientCmd)) {  // TODO: instead of using holds_alternative, we should use std::visit with
                                       // an overloaded lambda to avoid multiple parsing of the same command
                    std::cout << "Client " << message.clientId << " requested MSZ." << std::endl;

                    shared::protocol::server::Msz responseCmd{};
                    responseCmd.width = 16;   // TODO: get real map size from world
                    responseCmd.height = 16;  // TODO: get real map size from world
                    std::string const responseStr = shared::protocol::Emitter::build(responseCmd);

                    _sessionManager->sendMessage(message.clientId, responseStr);
                } else if (std::holds_alternative<shared::protocol::UnknownCommand>(clientCmd)) {
                    std::string const responseStr = shared::protocol::Emitter::build(shared::protocol::server::Suc{});
                    _sessionManager->sendMessage(message.clientId, responseStr);
                } else {
                    std::cout << "Received other message from client " << message.clientId << ": " << message.message
                              << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error in main loop: " << e.what() << std::endl;
        }
    }
}

void Core::stop() { _isRunning = false; }

}  // namespace zappy::server
