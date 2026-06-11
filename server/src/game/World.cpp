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
#include "util/DataStructures.hpp"

namespace zappy::server::game {

std::pair<std::size_t, std::size_t> World::getRandomPlace(const std::size_t heightMap, const std::size_t widthMap) {
    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<std::size_t> distHeight{0, heightMap};
    std::uniform_int_distribution<std::size_t> distWidth{0, widthMap};

    return {distHeight(e), distWidth(e)};
}

void World::setSpawnEggs(const std::size_t clientLimit, std::string_view teamName) {
    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<std::size_t> dist{0, _heightMap * _widthMap};

    for (std::size_t i = 0; i < clientLimit; i++) {
        const auto pos = dist(e);
        _vecEggs.push_back(Egg{.id = _newId, .pos = getTilePos(pos), .teamName = teamName.data()});
        _tiles.at(pos).eggs.push_back(_newId);
        _newId++;
    }
}

cardinalPoint World::randomCardinalPoint() {
    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<std::uint8_t> dist{0, static_cast<std::size_t>(cardinalPoint::COUNT)};

    return static_cast<cardinalPoint>(dist(e));
}

World::World(const util::Config& config) : _heightMap(config.height), _widthMap(config.width) {
    const auto nbTile = _heightMap * _widthMap;
    for (std::size_t i = 0; i < nbTile; i++) {
        _tiles.push_back(Tile{.ressources = {}});
    }
    for (const auto& teamName : config.teamNames) {
        _teamList[teamName] = std::make_unique<Team>(config.clientLimit);
        setSpawnEggs(config.clientLimit, teamName);
    }
}

std::pair<std::size_t, std::size_t> World::limitMap() const {
    return std::make_pair((_widthMap == 0 ? 0 : _widthMap - 1), (_heightMap == 0 ? 0 : _heightMap - 1));
}

void World::eraseEggFromTile(const std::size_t pos1dVec, const std::size_t id) {
    auto it = _tiles.at(pos1dVec).eggs.begin();
    while (it != _tiles.at(pos1dVec).eggs.end()) {
        if (*it == id) {
            _tiles.at(pos1dVec).eggs.erase(it);
            return;
        }
        ++it;
    }
}

std::optional<size_t> World::spawnPlayer(const std::string_view teamName) {
    const auto team = _teamList.find(teamName);
    if (team == _teamList.end()) {
        return std::nullopt;
    }
    const auto egg = _vecEggs.back();
    team->second->addInTeam(egg.id);
    _vecEggs.pop_back();
    eraseEggFromTile(getTileIndex(egg.pos), egg.id);
    _playerList.push_back(std::make_unique<Player>(egg.id, egg.pos.x, egg.pos.y, randomCardinalPoint()));
    const auto& newPlayer = _playerList.back();
    _tiles.at(getTileIndex(newPlayer->position().x, newPlayer->position().y)).players.push_back(newPlayer->id());
    return newPlayer->id();
}

[[nodiscard]] Pos World::getTilePos(const std::size_t pos) const {
    return Pos{.x = pos / _widthMap, .y = pos % _widthMap};
}

[[nodiscard]] std::size_t World::getTileIndex(const std::size_t x, const std::size_t y) const {
    return (y * _widthMap) + x;
}

[[nodiscard]] std::size_t World::getTileIndex(const Pos& pos) const { return (pos.y * _widthMap) + pos.x; }

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

void World::erasePlayerFromTile(const std::size_t pos1dVec, const std::size_t id) {
    auto it = _tiles.at(pos1dVec).players.begin();
    while (it != _tiles.at(pos1dVec).players.end()) {
        if (*it == id) {
            _tiles.at(pos1dVec).players.erase(it);
            return;
        }
        ++it;
    }
}

void World::updatePosOnMap(const std::size_t id, const Pos& oldPos, const Pos& newPos) {
    const auto oldTileIndex = getTileIndex(oldPos);
    const auto newTileIndex = getTileIndex(newPos);

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

std::vector<std::size_t> World::getDeadsId() const {
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
