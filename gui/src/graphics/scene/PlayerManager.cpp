/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** PlayerManager
*/

#include "PlayerManager.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <optional>
#include <string>

#include "Color.hpp"
#include "Tile3D.hpp"
#include "TileManager.hpp"
#include "gui/src/game/Player.hpp"
#include "gui/src/game/Team.hpp"
#include "protocol/Commands.hpp"

namespace zappy::gui::graphics::scene {
void PlayerManager::handleTeamName(const shared::protocol::server::Tna& command) {
    (void)ensureTeamExist(command.teamName);
}

void PlayerManager::handlePlayerConnected(const shared::protocol::server::Pnw& command) {
    if (!_tileManager.contains(command.x, command.y) || command.orientation < 1 || command.orientation > 4) {
        return;
    }

    auto position = _tileManager.tilePosition(command.x, command.y);
    position.setY(Tile3D::TILE_SIZE * DefaultPlayerOffsetY);
    auto& player = ensureTeamExist(command.teamName)
                       .addPlayer(command.playerId, position, orientationFromProtocol(command.orientation),
                                  static_cast<std::size_t>(std::max(command.level, 1)));
    player.setPosition(position);
    player.setTilePosition(command.x, command.y);
    player.setOrientation(orientationFromProtocol(command.orientation));
    player.setLevel(static_cast<std::size_t>(std::max(command.level, 1)));
}

void PlayerManager::handlePlayerPosition(const shared::protocol::server::Ppo& command) {
    if (!_tileManager.contains(command.x, command.y) || command.orientation < 1 || command.orientation > 4) {
        return;
    }
    if (const auto player = mutablePlayerById(command.playerId); player.has_value()) {
        updatePlayerPosition(player->get(), command.x, command.y);
        player->get().setOrientation(orientationFromProtocol(command.orientation));
    }
}

void PlayerManager::handlePlayerLevel(const shared::protocol::server::Plv& command) {
    if (command.level < 1) {
        return;
    }
    if (const auto player = mutablePlayerById(command.playerId); player.has_value()) {
        player->get().setLevel(static_cast<std::size_t>(command.level));
    }
}

void PlayerManager::handlePlayerInventory(const shared::protocol::server::Pin& command) {
    if (!_tileManager.contains(command.x, command.y)) {
        return;
    }
    const auto player = mutablePlayerById(command.playerId);
    if (!player.has_value()) {
        return;
    }
    updatePlayerPosition(player->get(), command.x, command.y);
    _tileManager.replaceResources(player->get().itemBag(), player->get().position(),
                                  {command.food, command.linemate, command.deraumere, command.sibur, command.mendiane,
                                   command.phiras, command.thystame});
}

void PlayerManager::handleIncantationStart(const shared::protocol::server::Pic& command) {
    if (!_tileManager.contains(command.x, command.y) || command.level < 1 || command.playerIds.empty()) {
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

void PlayerManager::handleIncantationEnd(const shared::protocol::server::Pie& command) {
    const auto incantation = std::ranges::find_if(_activeIncantations, [&command](const Incantation& active) {
        return active.x == command.x && active.y == command.y;
    });
    if (incantation == _activeIncantations.end()) {
        return;
    }
    if (command.incantationResult != 0) {
        for (const int playerId : incantation->playerIds) {
            if (const auto player = mutablePlayerById(playerId); player.has_value()) {
                const auto nextLevel = static_cast<std::size_t>(incantation->level) + 1U;
                player->get().setLevel(std::max(player->get().level(), nextLevel));
            }
        }
    }
    _activeIncantations.erase(incantation);
}

void PlayerManager::handleResourceDropped(const shared::protocol::server::Pdr& command) {
    const auto player = mutablePlayerById(command.playerId);
    if (!player.has_value()) {
        return;
    }
    const auto tile = _tileManager.mutableTileAt(player->get().tileX(), player->get().tileY());
    if (!tile.has_value()) {
        return;
    }
    TileManager::removeResource(player->get().itemBag(), command.resourceId);
    _tileManager.addResource(tile->get().itemBag(), tile->get().position(), command.resourceId);
}

void PlayerManager::handleResourceCollected(const shared::protocol::server::Pgt& command) {
    const auto player = mutablePlayerById(command.playerId);
    if (!player.has_value()) {
        return;
    }
    const auto tile = _tileManager.mutableTileAt(player->get().tileX(), player->get().tileY());
    if (!tile.has_value()) {
        return;
    }
    TileManager::removeResource(tile->get().itemBag(), command.resourceId);
    _tileManager.addResource(player->get().itemBag(), player->get().position(), command.resourceId);
}

void PlayerManager::handlePlayerDeath(const shared::protocol::server::Pdi& command) {
    if (const auto team = teamForPlayer(command.playerId); team.has_value()) {
        team->get().removePlayer(command.playerId);
    }
}

void PlayerManager::handleEggLaid(const shared::protocol::server::Enw& command) {
    if (!_tileManager.contains(command.x, command.y)) {
        return;
    }
    const auto team = teamForPlayer(command.playerId);
    if (!team.has_value()) {
        return;
    }
    auto position = _tileManager.tilePosition(command.x, command.y);
    position.setY(Tile3D::TILE_SIZE);
    team->get().addEgg(command.eggId, command.playerId, position);
}

void PlayerManager::handleEggRemoved(const shared::protocol::server::Ebo& command) { removeEgg(command.eggId); }

void PlayerManager::handleEggRemoved(const shared::protocol::server::Edi& command) { removeEgg(command.eggId); }

std::optional<std::reference_wrapper<const game::Player>> PlayerManager::playerById(const int id) const {
    for (const auto& team : _teams) {
        if (const auto player = team.findPlayer(id); player.has_value()) {
            return player;
        }
    }
    return std::nullopt;
}

void PlayerManager::updatePlayerName(const int id, const std::string& name) {
    if (const auto player = mutablePlayerById(id); player.has_value()) {
        player->get().setName(name);
    }
}

std::optional<std::reference_wrapper<game::Player>> PlayerManager::mutablePlayerById(const int id) {
    for (auto& team : _teams) {
        if (const auto player = team.findPlayer(id); player.has_value()) {
            return player;
        }
    }
    return std::nullopt;
}

std::optional<std::reference_wrapper<game::Team>> PlayerManager::teamForPlayer(const int playerId) {
    for (auto& team : _teams) {
        if (team.hasPlayer(playerId)) {
            return std::ref(team);
        }
    }
    return std::nullopt;
}

void PlayerManager::updatePlayerPosition(game::Player& player, const int x, const int y) const {
    auto position = _tileManager.tilePosition(x, y);
    position.setY(Tile3D::TILE_SIZE * DefaultPlayerOffsetY);
    player.setPosition(position);
    player.setTilePosition(x, y);
}

game::Team& PlayerManager::ensureTeamExist(const std::string& name) {
    const auto team =
        std::ranges::find_if(_teams, [&name](const game::Team& candidate) { return candidate.name() == name; });
    if (team != _teams.end()) {
        return *team;
    }
    return _teams.emplace_back(name, 0, nextTeamColor());
}

raylib::Color PlayerManager::nextTeamColor() const { return TeamColors.at(_teams.size() % TeamColors.size()); }

game::Player::cardinalPoint PlayerManager::orientationFromProtocol(const int orientation) {
    return static_cast<game::Player::cardinalPoint>(orientation);
}

void PlayerManager::removeEgg(const int eggId) {
    for (auto& team : _teams) {
        team.removeEgg(eggId);
    }
}
}  // namespace zappy::gui::graphics::scene
