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
#include "command/Unknown.hpp"
#include "exception/Exception.hpp"
#include "game/World.hpp"
#include "guiCommand/Mct.hpp"
#include "guiCommand/Tna.hpp"
#include "network/ISessionManager.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server {

Core::Core(const std::span<char*> args) : _args(args) {
    try {
        setup();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

int Core::run() {
    try {
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

void Core::stop() { _isRunning = false; }

void Core::loop() {
    auto nextTickTarget = std::chrono::steady_clock::now() + std::chrono::milliseconds{_timeUnit};

    while (_isRunning) {
        try {
            const auto now = std::chrono::steady_clock::now();
            const int nextExecutionTick = _world->getNextExecutionTick();

            if (nextExecutionTick == -1) {
                _sessionManager->pollNetwork(-1);
                processNetworkEvents();
                nextTickTarget = std::chrono::steady_clock::now() + std::chrono::milliseconds{_timeUnit};
                flushPlayerResponses();
                flushGuiResponses();
                continue;
            }
            int pollTimeout = -1;
            auto targetTime = nextTickTarget + std::chrono::milliseconds{_timeUnit * (nextExecutionTick - 1)};

            if (now < targetTime) {
                pollTimeout =
                    static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(targetTime - now).count());
            } else {
                pollTimeout = 0;
            }

            _sessionManager->pollNetwork(pollTimeout);
            processGameTick(nextTickTarget);
            processNetworkEvents();

            flushPlayerResponses();
            flushGuiResponses();
        } catch (const std::exception& e) {
            std::cerr << "Error in main loop: " << e.what() << std::endl;
        }
    }
}
void Core::processNetworkEvents() {
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

    while (now >= nextTickTarget) {
        _world->update();
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
    if (teamName == "GRAPHIC") {
        _clientStates[clientId] = ClientState::GUI;
        sendGuiInitialState(clientId);
        return;
    }

    const auto playerIdOpt = _world->spawnPlayer(teamName);
    if (!playerIdOpt.has_value()) {
        _sessionManager->sendMessage(clientId, "ko\n");
        return;
    }
    _clientToPlayer[clientId] = playerIdOpt.value();
    _clientStates[clientId] = ClientState::IN_GAME;
    _sessionManager->sendMessage(clientId, std::format("{}\n{} {}\n", _world->getAvailableSlotInTeam(teamName),
                                                       _world->sizeMap().x, _world->sizeMap().y));
    const auto& playerList = _world->playerList();
    _world->addGuiEvent(shared::protocol::Emitter::build(shared::protocol::server::Pnw{
        .playerId = static_cast<int>(playerIdOpt.value()),
        .x = static_cast<int>(playerList.at(playerIdOpt.value())->position().x),
        .y = static_cast<int>(playerList.at(playerIdOpt.value())->position().y),
        .orientation = static_cast<int>(playerList.at(playerIdOpt.value())->orientation()),
        .level = 1,
        .teamName = std::string{teamName},
    }));
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
        _world->pushCommandToPlayer(playerId, std::make_unique<command::Unknown>());
    }
}

void Core::handleGuiMessage(int clientId, std::string_view message) {
    auto command = _commandFactory.createGuiCommand(message);

    if (command != nullptr) {
        const auto response = command->execute(*this);

        _timeUnit = static_cast<int>(1.0F / static_cast<float>(_config.freq) * 1000);
        if (response.message.empty() || response.isArgumentError || response.isCommandError) {
            if (response.isArgumentError) {
                _sessionManager->sendMessage(clientId, "sbp\n");
            } else {
                _sessionManager->sendMessage(clientId, "suc\n");
            }
            return;
        }
        if (response.sendToEveryone) {
            for (const auto& [clientId, state] : _clientStates) {
                if (state == ClientState::GUI) {
                    _sessionManager->sendMessage(clientId, response.message);
                }
            }
            return;
        }
        _sessionManager->sendMessage(clientId, response.message);
    } else {
        _sessionManager->sendMessage(clientId, "suc\n");
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

void Core::sendGuiInitialState(int clientId) {
    const auto& eggs = _world->vecEggs();
    guiCommand::Mct mct{};
    guiCommand::Tna tna{};
    _sessionManager->sendMessage(clientId, shared::protocol::Emitter::build(shared::protocol::server::Msz{
                                               .width = static_cast<int>(_world->sizeMap().x),
                                               .height = static_cast<int>(_world->sizeMap().y)}));
    _sessionManager->sendMessage(
        clientId,
        shared::protocol::Emitter::build(shared::protocol::server::Sgt{.timeUnit = static_cast<int>(_config.freq)}));
    _sessionManager->sendMessage(clientId, mct.execute(*this).message);
    _sessionManager->sendMessage(clientId, tna.execute(*this).message);
    for (const auto& [playerId, player] : _world->playerList()) {
        _sessionManager->sendMessage(clientId, shared::protocol::Emitter::build(shared::protocol::server::Pnw{
                                                   .playerId = static_cast<int>(playerId),
                                                   .x = static_cast<int>(player->position().x),
                                                   .y = static_cast<int>(player->position().y),
                                                   .orientation = static_cast<int>(player->orientation()),
                                                   .level = player->level(),
                                                   .teamName = world().getPlayerTeam(playerId)}));
        // TODO: Uncomment this when the inventory is implemented
        // _sessionManager->sendMessage(clientId, shared::protocol::Emitter::build(shared::protocol::server::Pin{
        //                                            .playerId = static_cast<int>(playerId),
        //                                            .x = static_cast<int>(player->position().x),
        //                                            .y = static_cast<int>(player->position().y),
        //                                            .inventory = player->inventory()}));
        _sessionManager->sendMessage(clientId, shared::protocol::Emitter::build(shared::protocol::server::Plv{
                                                   .playerId = static_cast<int>(playerId), .level = player->level()}));
    }
    for (const auto& [eggId, egg] : eggs) {
        _sessionManager->sendMessage(clientId, shared::protocol::Emitter::build(shared::protocol::server::Enw{
                                                   .eggId = static_cast<int>(egg.id),
                                                   .playerId = -1,
                                                   .x = static_cast<int>(egg.position.x),
                                                   .y = static_cast<int>(egg.position.y)}));
    }
}

}  // namespace zappy::server
