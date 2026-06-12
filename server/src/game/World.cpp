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

void World::setSpawnEggs(const std::size_t clientLimit, const std::string_view teamName) {
    if (_heightMap == 0 || _widthMap == 0) {
        return;
    }
    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<std::size_t> dist{0, (_heightMap * _widthMap) - 1};

    for (std::size_t i = 0; i < clientLimit; i++) {
        const auto position = dist(e);
        _vecEggs[_newId] = Egg{.id = _newId, .position = getTilePosition(position), .teamName = std::string{teamName}};
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
        if (it->second.teamName == teamName) {
            Egg egg = it->second;
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
    _playerList[egg.value().id] =
        std::make_unique<Player>(egg.value().id, egg.value().position.x, egg.value().position.y, randomCardinalPoint());
    const auto& newPlayer = _playerList.at(egg.value().id);
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
    const auto& player = _playerList.at(playerId);

    player->pushCommand(std::move(command));
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
    for (const auto& player : _playerList | std::views::values) {
        player->update(*this);
    }
}

void World::removeFromMap(const std::size_t id) {
    const auto& player = _playerList.at(id);
    const auto tileIndex = getTileIndex(player->position());
    erasePlayerFromTile(tileIndex, id);
}

std::unordered_map<std::size_t, std::vector<std::string>> World::getAllResponsesBuffer() const {
    std::unordered_map<std::size_t, std::vector<std::string>> responses;
    for (const auto& player : _playerList | std::views::values) {
        if (!player->responses().empty()) {
            responses[player->id()] = player->responses();
        }
    }
    return responses;
}

std::size_t World::removePlayer(const std::size_t id) {
    const auto& player = _playerList.at(id);
    player->kill();
    removePlayerFromTeam(id);
    erasePlayerFromTile(getTileIndex(player->position()), id);
    return player->id();
}

std::vector<std::size_t> World::collectAndKillDeadPlayers() const {
    std::vector<std::size_t> deadIds;

    for (const auto& val : _playerList | std::views::values) {
        if (val->nbLifeTick() == 0) {
            val->kill();
            deadIds.push_back(val->id());
        }
    }
    return deadIds;
}

void World::eject(const std::size_t id) {
    const auto& pushingPlayer = _playerList.at(id);
    const auto orientation = pushingPlayer->orientation();
    const auto position = getTileIndex(pushingPlayer->position());
    const auto vecPlayerPush = _tiles.at(position).players;
    const auto vecEggPush = _tiles.at(position).eggs;

    for (const auto& idPushed : vecPlayerPush) {
        const auto& pushedPlayer = _playerList.at(idPushed);
        const auto [limitX, limitY] = sizeMap();

        const auto [oldX, oldY] = pushedPlayer->position();
        pushedPlayer->moveWithOrientation(Position{.x = limitX, .y = limitY}, orientation);
        const auto [newX, newY] = pushedPlayer->position();
        updatePositionOnMap(pushedPlayer->id(), {.x = oldX, .y = oldY}, {.x = newX, .y = newY});
    }
    for (const auto idEgg : vecEggPush) {
        _vecEggs.erase(idEgg);
        eraseEggFromTile(position, idEgg);
    }
}

}  // namespace zappy::server::game
