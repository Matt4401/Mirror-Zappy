/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Zappy core
*/

#include "Core.hpp"

#include <exception>
#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "SessionManager.hpp"
#include "network/ISessionManager.hpp"
#include "util/DataStructures.hpp"

namespace zappy::server {

Core::Core(util::Config config)
    : _config{std::move(config)},
      _sessionManager{std::make_unique<network::SessionManager>(_config.port)},
      _world{_config},
      _timeUnit{static_cast<int>(1.0F / static_cast<float>(_config.freq) * 1000)} {}

void Core::run() {
    shared::network::ISessionManager::NetworkEvent message{};

    while (_isRunning) {
        try {
            _sessionManager->pollNetwork(_timeUnit);
            while (_sessionManager->tryPopMessage(message)) {
                if (message.type == shared::network::ISessionManager::EventType::CLIENT_CONNECTED) {
                    handleNewClient(message.clientId);
                }
                if (message.type == shared::network::ISessionManager::EventType::CLIENT_DISCONNECTED) {
                    handleClientDisconnection(message.clientId);
                }
                if (message.type == shared::network::ISessionManager::EventType::MESSAGE_RECEIVED) {
                    handleClientMessage(message.clientId, message.message);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error in main loop: " << e.what() << std::endl;
        }
    }
}

void Core::stop() { _isRunning = false; }

void Core::handleNewClient(int clientId) {
    _sessionManager->sendMessage(clientId, "WELCOME\n");
    _clientStates.emplace(clientId, ClientState::WAITING_TEAM_SELECTION);
}

void Core::handleClientMessage(int clientId, std::string_view message) {
    auto it = _clientStates.find(clientId);
    if (it == _clientStates.end()) {
        return;
    }

    if (it->second == ClientState::WAITING_TEAM_SELECTION) {
        const auto teamName = std::string{message};
        const auto playerIdOpt = _world.spawnPlayer(teamName);

        if (!playerIdOpt.has_value()) {
            _clientToPlayer[clientId] = playerIdOpt.value();
        }
        _sessionManager->sendMessage(clientId, std::format("{}\n{} {}\n", _world.getAvailableSlotsInTeam(teamName),
        it->second = ClientState::IN_GAME;
    }
}

void Core::handleClientDisconnection(int clientId) {
    _clientStates.erase(clientId);
    _clientToPlayer.erase(clientId);
    _world.removePlayer(clientId);
}

}  // namespace zappy::server
