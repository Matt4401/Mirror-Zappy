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

Core::Core(util::Config& config) : _sessionManager(config.port), _config(std::move(config)) {}

void Core::run() {
    network::SessionManager::NetworkEvent message{};
    while (true) {
        _sessionManager.pollNetwork();
        while (_sessionManager.tryPopMessage(message)) {
            std::cout << "Received message from client " << message.clientId
                      << " of type: " << static_cast<int>(message.type) << ". Message: " << message.message
                      << std::endl;
        }
    }
}

}  // namespace zappy::server
