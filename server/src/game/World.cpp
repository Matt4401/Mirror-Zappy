/*
 ** EPITECH PROJECT, 2026
 ** zappy
 ** File description:
 ** World logic and storage
 */

#include "World.hpp"

#include <cmath>
#include <cstdint>
#include <ctime>
#include <memory>
#include <optional>
#include <random>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Player.hpp"
#include "Team.hpp"
#include "command/ICommand.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server::game {

Position World::getRandomPlace(const std::size_t heightMap, const std::size_t widthMap) {
    if (heightMap == 0 || widthMap == 0) {
        return Position{.x = 0, .y = 0};
    }
    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<std::size_t> distHeight{0, heightMap - 1};
    std::uniform_int_distribution<std::size_t> distWidth{0, widthMap - 1};

    return Position{.x = distHeight(e), .y = distWidth(e)};
}

void World::setSpawnEggs(const std::size_t clientLimit, const std::string_view teamName) {
    if (_heightMap == 0 || _widthMap == 0) {
        return;
    }
    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<std::size_t> dist{0, (_heightMap * _widthMap) - 1};

    for (std::size_t i = 0; i < clientLimit; i++) {
        const auto position = dist(e);
        _vecEggs.push_back(Egg{.id = _newId, .position = getTilePosition(position), .teamName = std::string{teamName}});
        _tiles.at(position).eggs.push_back(_newId);
        _newId++;
    }
}

cardinalPoint World::randomCardinalPoint() {
    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<std::uint8_t> dist{0, static_cast<std::uint8_t>(cardinalPoint::COUNT) - 1};

    return static_cast<cardinalPoint>(dist(e));
}

World::World(const parser::ServerConfig& config) : _heightMap(config.height), _widthMap(config.width) {
    const auto nbTile = _heightMap * _widthMap;
    for (std::size_t i = 0; i < nbTile; i++) {
        _tiles.push_back(Tile{.resources = {}});
    }
    for (const auto& teamName : config.teamNames) {
        _teamList[teamName] = std::make_unique<Team>(config.clientLimit);
        setSpawnEggs(config.clientLimit, teamName);
    }
}

Position World::sizeMap() const {
    return Position{.x = _widthMap == 0 ? 0 : _widthMap - 1, .y = _heightMap == 0 ? 0 : _heightMap - 1};
}

void World::eraseEggFromTile(const std::size_t position1dVec, const std::size_t id) {
    auto it = _tiles.at(position1dVec).eggs.begin();
    while (it != _tiles.at(position1dVec).eggs.end()) {
        if (*it == id) {
            _tiles.at(position1dVec).eggs.erase(it);
            return;
        }
        ++it;
    }
}

std::optional<Egg> World::getTeamEgg(const std::string_view& teamName) {
    auto it = _vecEggs.begin();
    while (it != _vecEggs.end()) {
        if (it->teamName == teamName) {
            Egg egg = *it;
            _vecEggs.erase(it);
            return egg;
        }
        ++it;
    }
    return std::nullopt;
}

std::optional<size_t> World::spawnPlayer(const std::string_view teamName) {
    const auto team = _teamList.find(std::string(teamName));
    if (team == _teamList.end() || _vecEggs.empty()) {
        return std::nullopt;
    }
    const auto& egg = getTeamEgg(teamName);
    if (!egg.has_value()) {
        return std::nullopt;
    }
    eraseEggFromTile(getTileIndex(egg.value().position), egg.value().id);
    team->second->addInTeam(egg.value().id);
    _playerList.push_back(std::make_unique<Player>(egg.value().id, egg.value().position.x, egg.value().position.y,
                                                   randomCardinalPoint()));
    const auto& newPlayer = _playerList.back();
    _tiles.at(getTileIndex(newPlayer->position().x, newPlayer->position().y)).players.push_back(newPlayer->id());
    return newPlayer->id();
}
[[nodiscard]] Position World::getTilePosition(const std::size_t position1D) const {
    return Position{.x = position1D % _widthMap, .y = position1D / _widthMap};
}

[[nodiscard]] std::size_t World::getTileIndex(const std::size_t x, const std::size_t y) const {
    return (y * _widthMap) + x;
}

[[nodiscard]] std::size_t World::getTileIndex(const Position& position) const {
    return (position.y * _widthMap) + position.x;
}

void World::pushCommandToPlayer(const std::size_t playerId, std::unique_ptr<command::ICommand> command) const {
    for (const auto& player : _playerList) {
        if (player->id() == playerId) {
            player->pushCommand(std::move(command));
            return;
        }
    }
}

void World::removePlayerFromTeam(const std::size_t id) const {
    for (const auto& val : _teamList | std::views::values) {
        if (val->isInTeam(id)) {
            val->removeFromTeam(id);
            return;
        }
    }
}

void World::erasePlayerFromTile(const std::size_t position1dVec, const std::size_t id) {
    auto it = _tiles.at(position1dVec).players.begin();
    while (it != _tiles.at(position1dVec).players.end()) {
        if (*it == id) {
            _tiles.at(position1dVec).players.erase(it);
            return;
        }
        ++it;
    }
}

void World::updatePositionOnMap(const std::size_t id, const Position& oldPosition, const Position& newPosition) {
    const auto oldTileIndex = getTileIndex(oldPosition);
    const auto newTileIndex = getTileIndex(newPosition);

    erasePlayerFromTile(oldTileIndex, id);
    _tiles.at(newTileIndex).players.push_back(id);
}

void World::update() {
    for (const auto& player : _playerList) {
        player->update(*this);
    }
}

void World::removeFromMap(const std::size_t id) {
    for (const auto& player : _playerList) {
        if (player->id() == id) {
            const auto tileIndex = getTileIndex(player->position());
            erasePlayerFromTile(tileIndex, id);
            return;
        }
    }
}

std::unordered_map<std::size_t, std::vector<std::string>> World::getAllResponsesBuffer() const {
    std::unordered_map<std::size_t, std::vector<std::string>> responses;
    for (const auto& player : _playerList) {
        if (!player->responses().empty()) {
            responses[player->id()] = player->responses();
        }
    }
    return responses;
}

std::optional<std::size_t> World::removePlayer(const std::size_t id) {
    for (const auto& player : _playerList) {
        if (player->id() == id) {
            player->kill();
            removePlayerFromTeam(id);
            erasePlayerFromTile(getTileIndex(player->position()), id);
            return player->id();
        }
    }
    return std::nullopt;
}

std::vector<std::size_t> World::collectAndKillDeadPlayers() const {
    std::vector<std::size_t> deadIds;

    for (const auto& player : _playerList) {
        if (player->nbLifeTick() == 0) {
            player->kill();
            deadIds.push_back(player->id());
        }
    }
    return deadIds;
}

}  // namespace zappy::server::game
