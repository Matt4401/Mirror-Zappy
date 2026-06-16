/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Zappy core
*/

#include "Core.hpp"

#include <chrono>
#include <exception>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Parser.hpp"
#include "SessionManager.hpp"
#include "exception/Exception.hpp"
#include "game/World.hpp"
#include "network/ISessionManager.hpp"
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

void Core::loop() {
    auto nextTickTarget = std::chrono::steady_clock::now() + std::chrono::milliseconds(_timeUnit);

    while (_isRunning) {
        try {
            const auto now = std::chrono::steady_clock::now();
            int pollTimeout = 0;

            if (now < nextTickTarget) {
                pollTimeout = static_cast<int>(
                    std::chrono::duration_cast<std::chrono::milliseconds>(nextTickTarget - now).count());
            }
            processNetworkEvents(pollTimeout);
            processGameTick(nextTickTarget);
        } catch (const std::exception& e) {
            std::cerr << "Error in main loop: " << e.what() << std::endl;
        }
    }
}

void Core::processNetworkEvents(int timeout) {
    _sessionManager->pollNetwork(timeout);
    shared::network::ISessionManager::NetworkEvent message{};

    while (_sessionManager->tryPopMessage(message)) {
        if (message.type == shared::network::ISessionManager::EventType::CLIENT_CONNECTED) {
            handleNewClient(message.clientId);
        }
        if (message.type == shared::network::ISessionManager::EventType::CLIENT_DISCONNECTED) {
            handleClientDisconnection(message.clientId);
        }
        if (message.type == shared::network::ISessionManager::EventType::MESSAGE_RECEIVED) {
            formatReceivedString(message.message);
            handleClientMessage(message.clientId, message.message);
        }
    }
}

void Core::processGameTick(std::chrono::steady_clock::time_point& nextTickTarget) {
    const auto now = std::chrono::steady_clock::now();

    if (now >= nextTickTarget) {
        _world->update();
        flushPlayerResponses();
        flushGuiResponses();

        nextTickTarget += std::chrono::milliseconds(_timeUnit);
    }
}

void Core::formatReceivedString(std::string& str) {
    if (!str.empty() && str.back() == '\n') {
        str.pop_back();
    }
    if (!str.empty() && str.back() == '\r') {
        str.pop_back();
    }
}

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
        handleHandshake(clientId, message);
    } else if (it->second == ClientState::IN_GAME) {
        handleInGameMessage(clientId, message);
    } else if (it->second == ClientState::GUI) {
        handleGuiMessage(clientId, message);
    }
}

void Core::handleHandshake(int clientId, std::string_view teamName) {
    const auto playerIdOpt = _world->spawnPlayer(teamName);

    if (teamName == "GRAPHIC") {
        _clientStates[clientId] = ClientState::GUI;
        return;
    }
    if (!playerIdOpt.has_value()) {
        _sessionManager->sendMessage(clientId, "ko\n");
        return;
    }
    _clientToPlayer[clientId] = playerIdOpt.value();
    _clientStates[clientId] = ClientState::IN_GAME;
    _sessionManager->sendMessage(clientId, std::format("{}\n{} {}\n", _world->getAvailableSlotInTeam(teamName),
                                                       _world->sizeMap().x, _world->sizeMap().y));
}

void Core::handleInGameMessage(int clientId, std::string_view message) {
    const auto playerIdIt = _clientToPlayer.find(clientId);
    if (playerIdIt == _clientToPlayer.end()) {
        return;
    }
    const auto playerId = playerIdIt->second;
    auto command = _commandFactory.createCommand(message);

    if (command != nullptr) {
        _world->pushCommandToPlayer(playerId, std::move(command));
    } else {
        _sessionManager->sendMessage(clientId, "ko\n");
    }
}

void Core::handleGuiMessage(int clientId, std::string_view message) {
    auto command = _commandFactory.createGuiCommand(message);

    if (command != nullptr) {
        const std::string response = command->execute(*_world);

        if (!response.empty()) {
            _sessionManager->sendMessage(clientId, response);
        }
    }
}

void Core::handleClientDisconnection(int clientId) {
    _clientStates.erase(clientId);
    if (!_clientToPlayer.contains(clientId)) {
        return;
    }
    _world->removePlayer(_clientToPlayer.at(clientId));
    _clientToPlayer.erase(clientId);
}

void Core::flushPlayerResponses() {
    const auto responses = _world->getAllResponsesBuffer();

    for (const auto& [clientId, playerId] : _clientToPlayer) {
        const auto it = responses.find(playerId);

        if (it == responses.end()) {
            continue;
        }
        for (const auto& message : it->second) {
            _sessionManager->sendMessage(clientId, message);
        }
    }
}

void Core::flushGuiResponses() {
    auto events = _world->getAndClearGuiEvents();
    if (events.empty()) {
        return;
    }

    for (const auto& [clientId, state] : _clientStates) {
        if (state == ClientState::GUI) {
            for (const auto& eventMsg : events) {
                _sessionManager->sendMessage(clientId, eventMsg);
            }
        }
    }
}

}  // namespace zappy::server
