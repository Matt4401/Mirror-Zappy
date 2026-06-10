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
#include "util/DataStructures.hpp"

namespace zappy::server {

Core::Core(util::Config config)
    : _config{std::move(config)},
      _sessionManager{std::make_unique<network::SessionManager>(_config.port)},
      _world{_config} {}

void Core::run() {
    shared::network::ISessionManager::NetworkEvent message{};

    while (_isRunning) {
        try {
            _sessionManager->pollNetwork();
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
