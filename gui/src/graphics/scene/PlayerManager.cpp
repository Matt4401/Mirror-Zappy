/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** PlayerManager
*/

#include "PlayerManager.hpp"

#include <raylib.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "Color.hpp"
#include "Tile3D.hpp"
#include "TileManager.hpp"
#include "events/GuiEvents.hpp"
#include "gui/src/game/Player.hpp"
#include "gui/src/game/Team.hpp"
#include "protocol/Commands.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::graphics::scene {
void PlayerManager::handleTeamName(const shared::protocol::server::Tna& command) {
    (void)ensureTeamExist(command.teamName);
}

void PlayerManager::handlePlayerConnected(const shared::protocol::server::Pnw& command) {
    const Tile3DPosition tilePosition{.x = command.x, .y = command.y};
    if (!_tileManager.contains(tilePosition) || command.orientation < 1 || command.orientation > 4) {
        return;
    }

    auto position = _tileManager.tilePosition(tilePosition);
    position.setY(Tile3D::TILE_SIZE * DefaultPlayerOffsetY);
    auto& player = ensureTeamExist(command.teamName)
                       .addPlayer(command.playerId, position, orientationFromProtocol(command.orientation),
                                  static_cast<std::size_t>(std::max(command.level, 1)));
    _dispatcher.get().dispatch(events::PlayerNameChanged{.playerId = command.playerId, .newName = player.name()});
    player.setPosition(position);
    player.setTilePosition(tilePosition);
    player.setOrientation(orientationFromProtocol(command.orientation));
    player.setLevel(static_cast<std::size_t>(std::max(command.level, 1)));
    recalculateTileOffsets(tilePosition);
}

void PlayerManager::handlePlayerPosition(const shared::protocol::server::Ppo& command) {
    const Tile3DPosition tilePosition{.x = command.x, .y = command.y};
    if (!_tileManager.contains(tilePosition) || command.orientation < 1 || command.orientation > 4) {
        return;
    }
    if (const auto player = playerById(command.playerId); player.has_value()) {
        const auto oldTilePosition = player->get().tilePosition();
        updatePlayerPosition(player->get(), tilePosition);
        player->get().turnToOrientation(orientationFromProtocol(command.orientation));
        recalculateTileOffsets(oldTilePosition);
        recalculateTileOffsets(tilePosition);
    }
}

void PlayerManager::handlePlayerLevel(const shared::protocol::server::Plv& command) {
    if (command.level < 1) {
        return;
    }
    if (const auto player = playerById(command.playerId); player.has_value()) {
        player->get().setLevel(static_cast<std::size_t>(command.level));
    }
}

void PlayerManager::handlePlayerInventory(const shared::protocol::server::Pin& command) {
    const Tile3DPosition tilePosition{.x = command.x, .y = command.y};
    if (!_tileManager.contains(tilePosition)) {
        return;
    }
    const auto player = playerById(command.playerId);
    if (!player.has_value()) {
        return;
    }
    updatePlayerPosition(player->get(), tilePosition);
    _tileManager.replaceResources(player->get().itemBag(), player->get().position(),
                                  {command.food, command.linemate, command.deraumere, command.sibur, command.mendiane,
                                   command.phiras, command.thystame});
}

void PlayerManager::handleIncantationStart(const shared::protocol::server::Pic& command) {
    if (!_tileManager.contains({.x = command.x, .y = command.y}) || command.level < 1 || command.playerIds.empty()) {
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

    for (const auto playerId : command.playerIds) {
        if (const auto player = playerById(static_cast<int>(playerId)); player.has_value()) {
            player->get().setAction(game::Player::Action::INCANTATION);
        }
    }
}

void PlayerManager::handleIncantationEnd(const shared::protocol::server::Pie& command) {
    const auto incantation = std::ranges::find_if(_activeIncantations, [&command](const Incantation& active) {
        return active.x == command.x && active.y == command.y;
    });
    if (incantation == _activeIncantations.end()) {
        return;
    }
    if (command.incantationResult) {
        for (const auto playerId : incantation->playerIds) {
            if (const auto player = playerById(static_cast<int>(playerId)); player.has_value()) {
                const auto nextLevel = static_cast<std::size_t>(incantation->level) + 1U;
                player->get().setLevel(std::max(player->get().level(), nextLevel));
            }
        }
    }
    for (const auto playerId : incantation->playerIds) {
        if (const auto player = playerById(static_cast<int>(playerId)); player.has_value()) {
            player->get().setAction(game::Player::Action::IDLE);
        }
    }
    _activeIncantations.erase(incantation);
}

void PlayerManager::handleResourceDropped(const shared::protocol::server::Pdr& command) {
    const auto player = playerById(command.playerId);
    if (!player.has_value()) {
        return;
    }
    const auto tile = _tileManager.tileAt(player->get().tilePosition());
    if (!tile.has_value()) {
        return;
    }
    TileManager::removeResource(player->get().itemBag(), command.resourceId);
    _tileManager.addResource(tile->get().itemBag(), tile->get().position(), command.resourceId);
}

void PlayerManager::handleResourceCollected(const shared::protocol::server::Pgt& command) {
    const auto player = playerById(command.playerId);
    if (!player.has_value()) {
        return;
    }
    const auto tile = _tileManager.tileAt(player->get().tilePosition());
    if (!tile.has_value()) {
        return;
    }
    TileManager::removeResource(tile->get().itemBag(), command.resourceId);
    _tileManager.addResource(player->get().itemBag(), player->get().position(), command.resourceId);
}

void PlayerManager::handlePlayerDeath(const shared::protocol::server::Pdi& command) {
    if (const auto player = playerById(command.playerId); player.has_value()) {
        const auto oldTilePosition = player->get().tilePosition();
        if (const auto team = teamForPlayer(command.playerId); team.has_value()) {
            team->get().removePlayer(command.playerId);
        }
        recalculateTileOffsets(oldTilePosition);
    }
}

void PlayerManager::handleEggLaid(const shared::protocol::server::Enw& command) {
    const Tile3DPosition tilePosition{.x = command.x, .y = command.y};
    if (!_tileManager.contains(tilePosition)) {
        return;
    }
    auto position = _tileManager.tilePosition(tilePosition);
    position.setY(Tile3D::TILE_SIZE);

    if (command.playerId < 0) {
        std::erase_if(_initialEggs, [&command](const InitialEgg& egg) { return egg.id == command.eggId; });
        _initialEggs.push_back({.id = command.eggId, .position = position});
        redistributeInitialEggs();
        return;
    }
    if (const auto team = teamForPlayer(command.playerId); team.has_value()) {
        team->get().addEgg(command.eggId, command.playerId, position);
    }
    if (const auto player = playerById(command.playerId); player.has_value()) {
        player->get().setAction(game::Player::Action::IDLE);
    }
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
    if (const auto player = playerById(id); player.has_value()) {
        player->get().setName(name);
    }
}

std::optional<std::reference_wrapper<game::Player>> PlayerManager::playerById(const int id) {
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

void PlayerManager::updatePlayerPosition(game::Player& player, const Tile3DPosition tilePosition) const {
    auto destination = _tileManager.tilePosition(tilePosition);
    destination.setY(Tile3D::TILE_SIZE * DefaultPlayerOffsetY);

    auto exitPosition = player.position();
    const bool wraps = wrapPositionIfNeeded(player, tilePosition, exitPosition);

    if (wraps) {
        player.setWrappedFuturePosition(exitPosition, destination);
    } else {
        player.setFuturePosition(destination);
    }
    player.setTilePosition(tilePosition);
}

bool PlayerManager::wrapPositionIfNeeded(const game::Player& player, const Tile3DPosition tilePosition,
                                         raylib::rmath::Vector3& exitPosition) const {
    const auto currentTile = player.tilePosition();

    if (_tileManager.width() > 1 && currentTile.y == tilePosition.y && currentTile.x == 0 &&
        tilePosition.x == _tileManager.width() - 1) {
        exitPosition.setX(exitPosition.x() - Tile3D::TILE_SIZE);
        return true;
    }
    if (_tileManager.width() > 1 && currentTile.y == tilePosition.y && currentTile.x == _tileManager.width() - 1 &&
        tilePosition.x == 0) {
        exitPosition.setX(exitPosition.x() + Tile3D::TILE_SIZE);
        return true;
    }
    if (_tileManager.height() > 1 && currentTile.x == tilePosition.x && currentTile.y == 0 &&
        tilePosition.y == _tileManager.height() - 1) {
        exitPosition.setZ(exitPosition.z() - Tile3D::TILE_SIZE);
        return true;
    }
    if (_tileManager.height() > 1 && currentTile.x == tilePosition.x && currentTile.y == _tileManager.height() - 1 &&
        tilePosition.y == 0) {
        exitPosition.setZ(exitPosition.z() + Tile3D::TILE_SIZE);
        return true;
    }
    return false;
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
    std::erase_if(_initialEggs, [eggId](const InitialEgg& egg) { return egg.id == eggId; });
    for (auto& team : _teams) {
        team.removeEgg(eggId);
    }
}

void PlayerManager::redistributeInitialEggs() {
    if (_teams.empty()) {
        return;
    }
    std::ranges::sort(_initialEggs, {}, &InitialEgg::id);
    for (const auto& egg : _initialEggs) {
        for (auto& team : _teams) {
            team.removeEgg(egg.id);
        }
    }

    const auto eggCount = _initialEggs.size();
    const auto teamCount = _teams.size();
    for (std::size_t index = 0; index < eggCount; index++) {
        const auto teamIndex = (index * teamCount) / eggCount;
        const auto& egg = _initialEggs.at(index);
        _teams.at(teamIndex).addEgg(egg.id, -1, egg.position);
    }
}

void PlayerManager::movePlayers(const int serverFrequency, const float deltaTime) {
    for (auto& team : _teams) {
        team.movePlayers(serverFrequency, deltaTime);
    }
}

void PlayerManager::recalculateTileOffsets(const Tile3DPosition tilePosition) {
    std::vector<std::reference_wrapper<game::Player>> playersOnTile;
    for (auto& team : _teams) {
        for (auto& player : team.players()) {
            if (player.tilePosition().x == tilePosition.x && player.tilePosition().y == tilePosition.y) {
                playersOnTile.push_back(std::ref(player));
            }
        }
    }

    const size_t count = playersOnTile.size();
    if (count == 0) {
        return;
    }

    const float radius = 0.35F * Tile3D::TILE_SIZE;

    for (size_t i = 0; i < count; ++i) {
        auto& player = playersOnTile.at(i).get();
        if (count == 1) {
            player.setTargetOffset({0.0F, 0.0F, 0.0F});
            if (!player.moving()) {
                player.snapOffset({0.0F, 0.0F, 0.0F});
            }
        } else {
            const float angle = (static_cast<float>(i) * 2.0F * PI) / static_cast<float>(count);
            const raylib::rmath::Vector3 offset = {std::cos(angle) * radius, 0.0F, std::sin(angle) * radius};
            player.setTargetOffset(offset);
            if (!player.moving()) {
                player.snapOffset(offset);
            }
        }
    }
}
}  // namespace zappy::gui::graphics::scene
