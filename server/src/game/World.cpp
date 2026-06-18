/*
 ** EPITECH PROJECT, 2026
 ** zappy
 ** File description:
 ** World logic and storage
 */

#include "World.hpp"

#include <array>
#include <cmath>
#include <cstdint>
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
        _tiles.at(position).eggs.emplace_back(_newId);
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
        _tiles.emplace_back(Tile{.resources = {}});
    }
    for (const auto& teamName : config.teamNames) {
        _teamList[teamName] = std::make_unique<Team>(config.clientLimit);
        setSpawnEggs(config.clientLimit, teamName);
    }
}

Position World::sizeMap() const {
    return Position{.x = _widthMap == 0 ? 0 : _widthMap, .y = _heightMap == 0 ? 0 : _heightMap};
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
    _tiles.at(getTileIndex(newPlayer->position().x, newPlayer->position().y)).players.emplace_back(newPlayer->id());
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
    _tiles.at(newTileIndex).players.emplace_back(id);
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
        if (auto vecResponses = player->responses(); !vecResponses.empty()) {
            responses[player->id()] = std::move(vecResponses);
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
            deadIds.emplace_back(val->id());
        }
    }
    return deadIds;
}

std::size_t World::getAvailableSlotInTeam(std::string_view teamName) const {
    const auto team = _teamList.find(std::string(teamName));

    if (team == _teamList.end()) {
        return 0;
    }
    return team->second->availableSlot();
}

void World::eject(const std::size_t id) {
    const auto& pushingPlayer = _playerList.at(id);
    const auto orientation = pushingPlayer->orientation();
    const auto position = getTileIndex(pushingPlayer->position());
    const auto& tile = _tiles.at(position);
    auto vecPlayerPush = tile.players;
    std::erase(vecPlayerPush, id);
    const auto vecEggPush = tile.eggs;

    for (const auto& idPushed : vecPlayerPush) {
        const auto& pushedPlayer = _playerList.at(idPushed);
        const auto [limitX, limitY] = sizeMap();

        const auto [oldX, oldY] = pushedPlayer->position();
        pushedPlayer->moveWithOrientation(Position{.x = limitX, .y = limitY}, orientation);
        const auto [newX, newY] = pushedPlayer->position();
        updatePositionOnMap(pushedPlayer->id(), {.x = oldX, .y = oldY}, {.x = newX, .y = newY});
        pushedPlayer->addResponse("eject: " + kCardinalPointToStr.at(orientation) + "\n");
    }
    for (const auto idEgg : vecEggPush) {
        _vecEggs.erase(idEgg);
        eraseEggFromTile(position, idEgg);
    }
    pushingPlayer->addResponse("ok\n");
}

bool World::hasEjectableTargetOnTile(const Position& position, const std::size_t id) const {
    auto tile = _tiles.at(getTileIndex(position));
    std::erase(tile.players, id);
    return !tile.players.empty() || !tile.eggs.empty();
}

bool World::isEggOnTile(const Position& position) const {
    const auto tile = _tiles.at(getTileIndex(position));
    return !tile.eggs.empty();
}

const std::unordered_map<std::size_t, std::unique_ptr<Player>>& World::playerList() const { return _playerList; }

std::vector<std::string> World::getAndClearGuiEvents() {
    std::vector<std::string> events = std::move(_guiEvents);

    _guiEvents.clear();
    return events;
}

int World::getNextExecutionTick() const {
    int nextTick = -1;

    for (const auto& player : _playerList | std::ranges::views::values) {
        int playerNextEvent = static_cast<int>(player->nbLifeTick());

        if (player->cmdTick() > 0 && player->cmdTick() < playerNextEvent) {
            playerNextEvent = player->cmdTick();
        } else if (player->cmdTick() == 0 && player->hasCommands()) {
            playerNextEvent = 1;
        }

        if (nextTick == -1 || playerNextEvent < nextTick) {
            nextTick = playerNextEvent;
        }
    }
    return nextTick;
}

void World::addItemOnGround(ItemType item, const Position pos) {
    _tiles.at(getTileIndex(pos)).resources.at(static_cast<std::uint8_t>(item))++;
}

void World::removeItemOnGround(ItemType item, const Position pos) {
    _tiles.at(getTileIndex(pos)).resources.at(static_cast<std::uint8_t>(item))--;
}

std::array<std::size_t, static_cast<uint8_t>(ItemType::COUNT)> World::tileResources(const Position position) const {
    return _tiles.at(getTileIndex(position)).resources;
}

}  // namespace zappy::server::game
