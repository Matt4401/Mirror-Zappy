/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** WorldManager
*/

#include "WorldManager.hpp"

#include <functional>
#include <memory>
#include <utility>

#include "events/EventDispatcher.hpp"
#include "protocol/Commands.hpp"

namespace zappy::gui::graphics::scene {
WorldManager::WorldManager(std::shared_ptr<events::EventDispatcher> dispatcher) : _dispatcher(std::move(dispatcher)) {
    if (!_dispatcher) {
        return;
    }
    subscribe<shared::protocol::server::Msz>(
        [this](const shared::protocol::server::Msz& command) { _tileManager.handleMapSize(command); });
    subscribe<shared::protocol::server::Bct>(
        [this](const shared::protocol::server::Bct& command) { _tileManager.handleTileContent(command); });
    subscribe<shared::protocol::server::Tna>(
        [this](const shared::protocol::server::Tna& command) { _playerManager.handleTeamName(command); });
    subscribe<shared::protocol::server::Pnw>(
        [this](const shared::protocol::server::Pnw& command) { _playerManager.handlePlayerConnected(command); });
    subscribe<shared::protocol::server::Ppo>(
        [this](const shared::protocol::server::Ppo& command) { _playerManager.handlePlayerPosition(command); });
    subscribe<shared::protocol::server::Plv>(
        [this](const shared::protocol::server::Plv& command) { _playerManager.handlePlayerLevel(command); });
    subscribe<shared::protocol::server::Pin>(
        [this](const shared::protocol::server::Pin& command) { _playerManager.handlePlayerInventory(command); });
    subscribe<shared::protocol::server::Pic>(
        [this](const shared::protocol::server::Pic& command) { _playerManager.handleIncantationStart(command); });
    subscribe<shared::protocol::server::Pie>(
        [this](const shared::protocol::server::Pie& command) { _playerManager.handleIncantationEnd(command); });
    subscribe<shared::protocol::server::Pdr>(
        [this](const shared::protocol::server::Pdr& command) { _playerManager.handleResourceDropped(command); });
    subscribe<shared::protocol::server::Pgt>(
        [this](const shared::protocol::server::Pgt& command) { _playerManager.handleResourceCollected(command); });
    subscribe<shared::protocol::server::Pdi>(
        [this](const shared::protocol::server::Pdi& command) { _playerManager.handlePlayerDeath(command); });
    subscribe<shared::protocol::server::Enw>(
        [this](const shared::protocol::server::Enw& command) { _playerManager.handleEggLaid(command); });
    subscribe<shared::protocol::server::Ebo>(
        [this](const shared::protocol::server::Ebo& command) { _playerManager.handleEggRemoved(command); });
    subscribe<shared::protocol::server::Edi>(
        [this](const shared::protocol::server::Edi& command) { _playerManager.handleEggRemoved(command); });
    subscribe<shared::protocol::server::Sgt>(
        [this](const shared::protocol::server::Sgt& command) { handleTimeUnit(command); });
    subscribe<shared::protocol::server::Sst>(
        [this](const shared::protocol::server::Sst& command) { handleTimeUnit(command); });
    subscribe<shared::protocol::server::Seg>(
        [this](const shared::protocol::server::Seg& command) { handleGameEnd(command); });
    subscribe<shared::protocol::server::Smg>(
        [this](const shared::protocol::server::Smg& command) { handleServerMessage(command); });
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
