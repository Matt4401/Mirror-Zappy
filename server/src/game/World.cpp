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
#include <random>
#include <ranges>
#include <string_view>
#include <utility>

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

void World::setSpawnEggs() {
    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<std::size_t> dist{0, _heightMap * _widthMap};

    for (std::size_t i = 0; i < _clientLimit; i++) {
        _vecEggs.push_back(dist(e));
    }
}

cardinalPoint World::randomCardinalPoint() {
    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<std::uint8_t> dist{0, static_cast<std::size_t>(cardinalPoint::COUNT)};

    return static_cast<cardinalPoint>(dist(e));
}

World::World(const util::Config& config)
    : _heightMap(config.height), _widthMap(config.width), _clientLimit(config.clientLimit) {
    for (const auto& teamName : config.teamNames) {
        _teamList[teamName] = std::make_unique<Team>(config.clientLimit);
    }
    const auto nbTile = _heightMap * _widthMap;
    for (std::size_t i = 0; i < nbTile; i++) {
        _tiles.push_back(Tile{.isEgg = false, .ressources = {}});
    }
}

std::pair<std::size_t, std::size_t> World::limitMap() { return std::make_pair(_heightMap, _widthMap); }

std::optional<size_t> World::spawnPlayer(const std::string_view teamName) {
    const auto team = _teamList.find(teamName);
    if (team == _teamList.end()) {
        return std::nullopt;
    }
    team->second->addInTeam(_newId);
    const auto pos = _vecEggs.back();
    _vecEggs.pop_back();
    _playerList.push_back(std::make_unique<Player>(_newId, pos / _widthMap, pos % _widthMap, randomCardinalPoint()));
    const auto& newPlayer = _playerList.back();
    _tiles.at(getTileIndex(newPlayer->position().x, newPlayer->position().y)).players.push_back(newPlayer->id());
    _newId++;
    return newPlayer->id();
}

[[nodiscard]] std::size_t World::getTileIndex(const int x, const int y) const { return (y * _widthMap) + x; }

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

void World::erasePlayerFromTiles(const std::size_t pos1dVec, const std::size_t id) {
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

    erasePlayerFromTiles(oldTileIndex, id);
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
            erasePlayerFromTiles(tileIndex, id);
            return;
        }
    }
}

std::optional<std::size_t> World::Kill(const std::size_t id) {
    for (const auto& player : _playerList) {
        if (player->id() == id) {
            player->kill();
            removePlayerFromTeam(id);
            erasePlayerFromTiles(getTileIndex(player->position()), id);
            return player->id();
        }
    }
    return std::nullopt;
}

std::vector<std::size_t> World::removeDead() {
    std::vector<std::size_t> deadIds;

    for (const auto& player : _playerList) {
        if (player->nbLifeTick() == 0) {
            player->kill();
            removePlayerFromTeam(player->id());
            removeFromMap(player->id());
            deadIds.push_back(player->id());
        }
    }
    return deadIds;
}

}  // namespace zappy::server::game
