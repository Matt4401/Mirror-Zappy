/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Zappy core
*/

#include "Core.hpp"

#include <iostream>
#include <utility>

#include "network/SessionManager.hpp"
#include "util/DataStructures.hpp"

namespace zappy::server {

Core::Core(util::ServerConfig config) : _config(std::move(config)), _sessionManager(_config.port), _world(_config) {}

void Core::run() {
    network::SessionManager::NetworkEvent message{};
    while (_isRunning) {
        _sessionManager.pollNetwork();
        while (_sessionManager.tryPopMessage(message)) {
            // TODO: should be removed, is of course temporary for testing purposes
            std::cout << "Received message from client " << message.clientId
                      << " of type: " << static_cast<int>(message.type) << ". Message: " << message.message
                      << std::endl;
        }
    }
}

void Core::stop() { _isRunning = false; }

}  // namespace zappy::server
