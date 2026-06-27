/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** WorldManager
*/

#include "WorldManager.hpp"

#include <functional>
#include <memory>

#include "AudioManager.hpp"
#include "PlayerManager.hpp"
#include "TileManager.hpp"
#include "events/EventDispatcher.hpp"
#include "protocol/Commands.hpp"

namespace zappy::gui::graphics::scene {
WorldManager::WorldManager(events::EventDispatcher& dispatcher, AudioManager& audioManager)
    : _dispatcher(dispatcher), _audioManager(audioManager), _tileManager(std::make_unique<TileManager>()) {
    _playerManager = std::make_unique<PlayerManager>(*_tileManager, _dispatcher.get(), _audioManager.get());
    initMapSubscriptions();
    initPlayerSubscriptions();
    initResourceSubscriptions();
    initEggSubscriptions();
    initGameSubscriptions();
}

void WorldManager::initMapSubscriptions() {
    registerHandler(_tileManager.get(), &TileManager::handleMapSize);
    registerHandler(_tileManager.get(), &TileManager::handleTileContent);
}

void WorldManager::initPlayerSubscriptions() {
    registerHandler(_playerManager.get(), &PlayerManager::handleTeamName);
    registerHandler(_playerManager.get(), &PlayerManager::handlePlayerConnected);
    registerHandler(_playerManager.get(), &PlayerManager::handlePlayerPosition);
    registerHandler(_playerManager.get(), &PlayerManager::handlePlayerLevel);
    registerHandler(_playerManager.get(), &PlayerManager::handlePlayerInventory);
    registerHandler(_playerManager.get(), &PlayerManager::handlePlayerDeath);
    registerHandler(_playerManager.get(), &PlayerManager::handleExpulsionAnimation);
}

void WorldManager::initResourceSubscriptions() {
    registerHandler(_playerManager.get(), &PlayerManager::handleIncantationStart);
    registerHandler(_playerManager.get(), &PlayerManager::handleIncantationEnd);
    registerHandler(_playerManager.get(), &PlayerManager::handleResourceDropped);
    registerHandler(_playerManager.get(), &PlayerManager::handleResourceCollected);
}

void WorldManager::initEggSubscriptions() {
    registerHandler(_playerManager.get(), &PlayerManager::handleEggLaid);
    registerHandler(_playerManager.get(), &PlayerManager::handleEggDropAnimation);
    subscribe<shared::protocol::server::Ebo>(
        [this](const shared::protocol::server::Ebo& command) { _playerManager->handleEggRemoved(command); });
    subscribe<shared::protocol::server::Edi>(
        [this](const shared::protocol::server::Edi& command) { _playerManager->handleEggRemoved(command); });
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
        _audioManager.get().setServerFrequency(_timeUnit);
    }
}

void WorldManager::handleTimeUnit(const shared::protocol::server::Sst& command) {
    if (command.timeUnit > 0) {
        _timeUnit = command.timeUnit;
        _audioManager.get().setServerFrequency(_timeUnit);
    }
}

void WorldManager::handleGameEnd(const shared::protocol::server::Seg& command) { _winningTeam = command.teamName; }

void WorldManager::handleServerMessage(const shared::protocol::server::Smg& command) {
    _lastServerMessage = command.message;
}
}  // namespace zappy::gui::graphics::scene
