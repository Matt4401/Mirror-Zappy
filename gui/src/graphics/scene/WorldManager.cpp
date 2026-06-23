/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** WorldManager
*/

#include "WorldManager.hpp"

#include <functional>

#include "PlayerManager.hpp"
#include "TileManager.hpp"
#include "events/EventDispatcher.hpp"
#include "protocol/Commands.hpp"

namespace zappy::gui::graphics::scene {
WorldManager::WorldManager(events::EventDispatcher& dispatcher) : _dispatcher(dispatcher) {
    initMapSubscriptions();
    initPlayerSubscriptions();
    initResourceSubscriptions();
    initEggSubscriptions();
    initGameSubscriptions();
}

void WorldManager::initMapSubscriptions() {
    registerHandler(&_tileManager, &TileManager::handleMapSize);
    registerHandler(&_tileManager, &TileManager::handleTileContent);
}

void WorldManager::initPlayerSubscriptions() {
    registerHandler(&_playerManager, &PlayerManager::handleTeamName);
    registerHandler(&_playerManager, &PlayerManager::handlePlayerConnected);
    registerHandler(&_playerManager, &PlayerManager::handlePlayerPosition);
    registerHandler(&_playerManager, &PlayerManager::handlePlayerLevel);
    registerHandler(&_playerManager, &PlayerManager::handlePlayerInventory);
    registerHandler(&_playerManager, &PlayerManager::handlePlayerDeath);
}

void WorldManager::initResourceSubscriptions() {
    registerHandler(&_playerManager, &PlayerManager::handleIncantationStart);
    registerHandler(&_playerManager, &PlayerManager::handleIncantationEnd);
    registerHandler(&_playerManager, &PlayerManager::handleResourceDropped);
    registerHandler(&_playerManager, &PlayerManager::handleResourceCollected);
}

void WorldManager::initEggSubscriptions() {
    registerHandler(&_playerManager, &PlayerManager::handleEggLaid);
    subscribe<shared::protocol::server::Ebo>(
        [this](const shared::protocol::server::Ebo& command) { _playerManager.handleEggRemoved(command); });
    subscribe<shared::protocol::server::Edi>(
        [this](const shared::protocol::server::Edi& command) { _playerManager.handleEggRemoved(command); });
}

void WorldManager::initGameSubscriptions() {
    subscribe<shared::protocol::server::Sgt>(
        [this](const shared::protocol::server::Sgt& command) { handleTimeUnit(command); });
    subscribe<shared::protocol::server::Sst>(
        [this](const shared::protocol::server::Sst& command) { handleTimeUnit(command); });
    registerHandler(this, &WorldManager::handleGameEnd);
    registerHandler(this, &WorldManager::handleServerMessage);
}

WorldManager::~WorldManager() {
    for (const auto& unsubscribe : _unsubscribers) {
        unsubscribe();
    }
}

void WorldManager::handleTimeUnit(const shared::protocol::server::Sgt& command) {
    if (command.timeUnit > 0) {
        _timeUnit = command.timeUnit;
    }
}

void WorldManager::handleTimeUnit(const shared::protocol::server::Sst& command) {
    if (command.timeUnit > 0) {
        _timeUnit = command.timeUnit;
    }
}

void WorldManager::handleGameEnd(const shared::protocol::server::Seg& command) { _winningTeam = command.teamName; }

void WorldManager::handleServerMessage(const shared::protocol::server::Smg& command) {
    _lastServerMessage = command.message;
}
}  // namespace zappy::gui::graphics::scene
