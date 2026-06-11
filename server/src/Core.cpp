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
#include <utility>

#include "SessionManager.hpp"
#include "network/ISessionManager.hpp"
#include "parsing/Parser.hpp"
#include "parsing/strategy/ServerStrategy.hpp"

namespace zappy::server {

Core::Core(const std::span<char*> args) : _args(args) {}

int Core::run() {
    try {
        setup();
        loop();
    } catch (const zappy::shared::exception::Exception& e) {
        if (std::string(e.what()) == zappy::parser::parsing::kUsageThrowMessage) {
            return zappy::parser::parsing::kExitSuccess;
        }
        std::cerr << "Error: " << e.what() << std::endl;
        return zappy::parser::parsing::kExitFailure;
    } catch (const std::exception& e) {
        std::cerr << "Unknown error: " << e.what() << std::endl;
        return zappy::parser::parsing::kExitFailure;
    }
    return zappy::parser::parsing::kExitSuccess;
}

void Core::setup() {
    auto serverStrategy = std::make_unique<zappy::parser::parsing::ServerStrategy>();
    zappy::parser::parsing::Parser<zappy::parser::parsing::ServerConfig> parser(std::move(serverStrategy));

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
                // TODO: should be removed, is of course temporary for testing purposes
                std::cout << "Received message from client " << message.clientId
                          << " of type: " << static_cast<int>(message.type) << ". Message: " << message.message
                          << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error in main loop: " << e.what() << std::endl;
        }
    }
}

void Core::stop() { _isRunning = false; }

}  // namespace zappy::server
