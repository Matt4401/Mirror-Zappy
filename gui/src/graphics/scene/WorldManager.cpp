/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** WorldManager
*/

#include "WorldManager.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "Color.hpp"
#include "Tile3D.hpp"
#include "events/EventDispatcher.hpp"
#include "game/Team.hpp"
#include "game/components/IObject.hpp"
#include "gui/src/game/ItemBag.hpp"
#include "gui/src/game/Player.hpp"
#include "protocol/Commands.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::graphics::scene {
WorldManager::WorldManager(std::shared_ptr<events::EventDispatcher> dispatcher) : _dispatcher(std::move(dispatcher)) {
    if (!_dispatcher) {
        return;
    }
    subscribe<shared::protocol::server::Msz>(
        [this](const shared::protocol::server::Msz& command) { handleMapSize(command); });
    subscribe<shared::protocol::server::Bct>(
        [this](const shared::protocol::server::Bct& command) { handleTileContent(command); });
    subscribe<shared::protocol::server::Tna>(
        [this](const shared::protocol::server::Tna& command) { handleTeamName(command); });
    subscribe<shared::protocol::server::Pnw>(
        [this](const shared::protocol::server::Pnw& command) { handlePlayerConnected(command); });
    subscribe<shared::protocol::server::Ppo>(
        [this](const shared::protocol::server::Ppo& command) { handlePlayerPosition(command); });
    subscribe<shared::protocol::server::Plv>(
        [this](const shared::protocol::server::Plv& command) { handlePlayerLevel(command); });
    subscribe<shared::protocol::server::Pin>(
        [this](const shared::protocol::server::Pin& command) { handlePlayerInventory(command); });
    subscribe<shared::protocol::server::Pic>(
        [this](const shared::protocol::server::Pic& command) { handleIncantationStart(command); });
    subscribe<shared::protocol::server::Pie>(
        [this](const shared::protocol::server::Pie& command) { handleIncantationEnd(command); });
    subscribe<shared::protocol::server::Pdr>(
        [this](const shared::protocol::server::Pdr& command) { handleResourceDropped(command); });
    subscribe<shared::protocol::server::Pgt>(
        [this](const shared::protocol::server::Pgt& command) { handleResourceCollected(command); });
    subscribe<shared::protocol::server::Pdi>(
        [this](const shared::protocol::server::Pdi& command) { handlePlayerDeath(command); });
    subscribe<shared::protocol::server::Enw>(
        [this](const shared::protocol::server::Enw& command) { handleEggLaid(command); });
    subscribe<shared::protocol::server::Ebo>(
        [this](const shared::protocol::server::Ebo& command) { handleEggRemoved(command); });
    subscribe<shared::protocol::server::Edi>(
        [this](const shared::protocol::server::Edi& command) { handleEggRemoved(command); });
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

void WorldManager::handleMapSize(const shared::protocol::server::Msz& command) {
    _width = std::max(command.width, 0);
    _height = std::max(command.height, 0);
    _tiles.clear();
    _tiles.reserve(static_cast<std::size_t>(_width) * static_cast<std::size_t>(_height));

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            _tiles.emplace_back(x, y, tilePosition(x, y));
        }
    }
}

void WorldManager::handleTileContent(const shared::protocol::server::Bct& command) {
    const auto tile = mutableTileAt(command.x, command.y);
    if (!tile.has_value()) {
        return;
    }

    auto& bag = tile->get().itemBag();
    const auto position = tile->get().position();
    replaceResources(bag, position,
                     {command.food, command.linemate, command.deraumere, command.sibur, command.mendiane,
                      command.phiras, command.thystame});
}

void WorldManager::handleTeamName(const shared::protocol::server::Tna& command) {
    (void)ensureTeamExist(command.teamName);
}

void WorldManager::handlePlayerConnected(const shared::protocol::server::Pnw& command) {
    if (!contains(command.x, command.y) || command.orientation < 1 || command.orientation > 4) {
        return;
    }

    auto position = tilePosition(command.x, command.y);
    position.setY(Tile3D::TILE_SIZE * DefaultPlayerOffsetY);
    auto& player = ensureTeamExist(command.teamName)
                       .addPlayer(command.playerId, position, orientationFromProtocol(command.orientation),
                                  static_cast<std::size_t>(std::max(command.level, 1)));
    player.setPosition(position);
    player.setTilePosition(command.x, command.y);
    player.setOrientation(orientationFromProtocol(command.orientation));
    player.setLevel(static_cast<std::size_t>(std::max(command.level, 1)));
}

void WorldManager::handlePlayerPosition(const shared::protocol::server::Ppo& command) {
    if (!contains(command.x, command.y) || command.orientation < 1 || command.orientation > 4) {
        return;
    }
    if (const auto player = mutablePlayerById(command.playerId); player.has_value()) {
        updatePlayerPosition(player->get(), command.x, command.y);
        player->get().setOrientation(orientationFromProtocol(command.orientation));
    }
}

void WorldManager::handlePlayerLevel(const shared::protocol::server::Plv& command) {
    if (command.level < 1) {
        return;
    }
    if (const auto player = mutablePlayerById(command.playerId); player.has_value()) {
        player->get().setLevel(static_cast<std::size_t>(command.level));
    }
}

void WorldManager::handlePlayerInventory(const shared::protocol::server::Pin& command) {
    if (!contains(command.x, command.y)) {
        return;
    }
    const auto player = mutablePlayerById(command.playerId);
    if (!player.has_value()) {
        return;
    }
    updatePlayerPosition(player->get(), command.x, command.y);
    replaceResources(player->get().itemBag(), player->get().position(),
                     {command.food, command.linemate, command.deraumere, command.sibur, command.mendiane,
                      command.phiras, command.thystame});
}

void WorldManager::handleIncantationStart(const shared::protocol::server::Pic& command) {
    if (!contains(command.x, command.y) || command.level < 1 || command.playerIds.empty()) {
        return;
    }
    const auto incantation = std::ranges::find_if(_activeIncantations, [&command](const Incantation& active) {
        return active.x == command.x && active.y == command.y;
    });
    const Incantation next{
        .x = command.x,
        .y = command.y,
        .level = command.level,
        .playerIds = command.playerIds,
    };
    if (incantation == _activeIncantations.end()) {
        _activeIncantations.push_back(next);
    } else {
        *incantation = next;
    }
}

void WorldManager::handleIncantationEnd(const shared::protocol::server::Pie& command) {
    const auto incantation = std::ranges::find_if(_activeIncantations, [&command](const Incantation& active) {
        return active.x == command.x && active.y == command.y;
    });
    if (incantation == _activeIncantations.end()) {
        return;
    }
    if (command.incantationResult != 0) {
        for (const int playerId : incantation->playerIds) {
            if (const auto player = mutablePlayerById(playerId); player.has_value()) {
                const auto nextLevel = static_cast<std::size_t>(incantation->level) + 1;
                player->get().setLevel(std::max(player->get().level(), nextLevel));
            }
        }
    }
    _activeIncantations.erase(incantation);
}

void WorldManager::handleResourceDropped(const shared::protocol::server::Pdr& command) {
    const auto player = mutablePlayerById(command.playerId);
    if (!player.has_value()) {
        return;
    }
    const auto tile = mutableTileAt(player->get().tileX(), player->get().tileY());
    if (!tile.has_value() || !resourceName(command.resourceId).has_value()) {
        return;
    }
    removeResource(player->get().itemBag(), command.resourceId);
    addResource(tile->get().itemBag(), tile->get().position(), command.resourceId);
}

void WorldManager::handleResourceCollected(const shared::protocol::server::Pgt& command) {
    const auto player = mutablePlayerById(command.playerId);
    if (!player.has_value()) {
        return;
    }
    const auto tile = mutableTileAt(player->get().tileX(), player->get().tileY());
    if (!tile.has_value() || !resourceName(command.resourceId).has_value()) {
        return;
    }
    removeResource(tile->get().itemBag(), command.resourceId);
    addResource(player->get().itemBag(), player->get().position(), command.resourceId);
}

void WorldManager::handlePlayerDeath(const shared::protocol::server::Pdi& command) {
    if (const auto team = teamForPlayer(command.playerId); team.has_value()) {
        team->get().removePlayer(command.playerId);
    }
}

void WorldManager::handleEggLaid(const shared::protocol::server::Enw& command) {
    if (!contains(command.x, command.y)) {
        return;
    }
    const auto team = teamForPlayer(command.playerId);
    if (!team.has_value()) {
        return;
    }
    auto position = tilePosition(command.x, command.y);
    position.setY(Tile3D::TILE_SIZE);
    team->get().addEgg(command.eggId, command.playerId, position);
}

void WorldManager::handleEggRemoved(const shared::protocol::server::Ebo& command) { removeEgg(command.eggId); }

void WorldManager::handleEggRemoved(const shared::protocol::server::Edi& command) { removeEgg(command.eggId); }

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

std::optional<std::reference_wrapper<const Tile3D>> WorldManager::tileAt(int x, int y) const {
    if (!contains(x, y)) {
        return std::nullopt;
    }
    const auto index = (static_cast<std::size_t>(y) * static_cast<std::size_t>(_width)) + static_cast<std::size_t>(x);
    return std::cref(_tiles.at(index));
}

std::optional<std::reference_wrapper<const game::Player>> WorldManager::playerById(const int id) const {
    for (const auto& team : _teams) {
        if (const auto player = team.findPlayer(id); player.has_value()) {
            return player;
        }
    }
    return std::nullopt;
}

void WorldManager::updatePlayerName(const int id, const std::string& name) {
    if (const auto player = mutablePlayerById(id); player.has_value()) {
        player->get().setName(name);
    }
}

std::optional<std::reference_wrapper<Tile3D>> WorldManager::mutableTileAt(int x, int y) {
    if (!contains(x, y)) {
        return std::nullopt;
    }
    const auto index = (static_cast<std::size_t>(y) * static_cast<std::size_t>(_width)) + static_cast<std::size_t>(x);
    return std::ref(_tiles.at(index));
}

std::optional<std::reference_wrapper<game::Player>> WorldManager::mutablePlayerById(const int id) {
    for (auto& team : _teams) {
        if (const auto player = team.findPlayer(id); player.has_value()) {
            return player;
        }
    }
    return std::nullopt;
}

std::optional<std::reference_wrapper<game::Team>> WorldManager::teamForPlayer(const int playerId) {
    for (auto& team : _teams) {
        if (team.hasPlayer(playerId)) {
            return std::ref(team);
        }
    }
    return std::nullopt;
}

bool WorldManager::contains(int x, int y) const { return x >= 0 && y >= 0 && x < _width && y < _height; }

raylib::rmath::Vector3 WorldManager::tilePosition(int x, int y) const {
    const float offsetX = static_cast<float>(_width - 1) * Tile3D::TILE_SIZE * 0.5F;
    const float offsetZ = static_cast<float>(_height - 1) * Tile3D::TILE_SIZE * 0.5F;
    return {(static_cast<float>(x) * Tile3D::TILE_SIZE) - offsetX, 0.0F,
            (static_cast<float>(y) * Tile3D::TILE_SIZE) - offsetZ};
}

void WorldManager::updatePlayerPosition(game::Player& player, const int x, const int y) {
    auto position = tilePosition(x, y);
    position.setY(Tile3D::TILE_SIZE * 1.4F);
    player.setPosition(position);
    player.setTilePosition(x, y);
}

game::Team& WorldManager::ensureTeamExist(const std::string& name) {
    const auto team =
        std::ranges::find_if(_teams, [&name](const game::Team& candidate) { return candidate.name() == name; });

    if (team != _teams.end()) {
        return *team;
    }
    return _teams.emplace_back(name, 0, nextTeamColor());
}

raylib::Color WorldManager::nextTeamColor() const { return TeamColors.at(_teams.size() % TeamColors.size()); }

game::Player::cardinalPoint WorldManager::orientationFromProtocol(int orientation) {
    return static_cast<game::Player::cardinalPoint>(orientation);
}

std::optional<std::string_view> WorldManager::resourceName(const int resourceId) {
    static constexpr std::array<std::string_view, 7> ResourceNames = {
        "Food", "Linemate", "Deraumere", "Sibur", "Mendiane", "Phiras", "Thystame",
    };
    if (resourceId < 0 || std::cmp_greater_equal(resourceId, ResourceNames.size())) {
        return std::nullopt;
    }
    return ResourceNames.at(static_cast<std::size_t>(resourceId));
}

std::unique_ptr<game::IObject> WorldManager::makeResource(const int resourceId,
                                                          const raylib::rmath::Vector3& position) {
    if (const auto factory = _resourceFactories.find(resourceId); factory != _resourceFactories.end()) {
        return factory->second(position);
    }
    return nullptr;
}

void WorldManager::replaceResources(game::ItemBag& bag, const raylib::rmath::Vector3& position,
                                    const std::array<int, 7>& quantities) {
    bag.clear();
    for (std::size_t resourceId = 0; resourceId < quantities.size(); resourceId++) {
        if (quantities.at(resourceId) > 0) {
            addResource(bag, position, static_cast<int>(resourceId),
                        static_cast<std::size_t>(quantities.at(resourceId)));
        }
    }
}

void WorldManager::addResource(game::ItemBag& bag, const raylib::rmath::Vector3& position, const int resourceId,
                               const std::size_t quantity) {
    if (auto resource = makeResource(resourceId, position); resource) {
        bag.addItem(std::move(resource), quantity);
    }
}

void WorldManager::removeResource(game::ItemBag& bag, const int resourceId, const std::size_t quantity) {
    if (const auto name = resourceName(resourceId); name.has_value()) {
        bag.removeItem(name.value(), quantity);
    }
}

void WorldManager::removeEgg(const int eggId) {
    for (auto& team : _teams) {
        team.removeEgg(eggId);
    }
}

}  // namespace zappy::gui::graphics::scene
