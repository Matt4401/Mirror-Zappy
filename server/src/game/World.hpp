/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World logic and storage
*/

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Team.hpp"
#include "command/ICommand.hpp"
#include "game/Player.hpp"
#include "util/DataStructures.hpp"

namespace zappy::server::game {

struct Egg {
    std::size_t id;
    Pos pos;
    std::string teamName;
};

struct Tile {
    std::vector<std::size_t> players;
    std::vector<std::size_t> eggs;
    std::array<std::size_t, static_cast<uint8_t>(ItemType::COUNT)> resources;
};

class World {
  public:
    static std::pair<std::size_t, std::size_t> getRandomPlace(std::size_t heightMap, std::size_t widthMap);
    void setSpawnEggs(size_t clientLimit, std::string_view teamName);
    static cardinalPoint randomCardinalPoint();
    explicit World(const util::Config& config);
    ~World() = default;

    World(const World& other) = delete;
    World& operator=(const World& other) = delete;
    World(World&& other) = delete;
    World& operator=(World&& other) = delete;

    std::pair<std::size_t, std::size_t> limitMap() const;
    [[nodiscard]] std::optional<size_t> spawnPlayer(std::string_view teamName);
    Pos getTilePos(std::size_t pos) const;

    void update();
    void removeFromMap(std::size_t id);
    std::unordered_map<std::size_t, std::vector<std::string>> getAllResponsesBuffer() const;
    void pushCommandToPlayer(std::size_t playerId, std::unique_ptr<command::ICommand> command) const;
    void removePlayerFromTeam(std::size_t id) const;
    void updatePosOnMap(std::size_t id, const Pos& oldPos, const Pos& newPos);

    std::optional<std::size_t> removePlayer(std::size_t id);
    std::vector<std::size_t> collectAndKillDeadPlayers() const;

  private:
    std::unordered_map<std::string, std::unique_ptr<Team>> _teamList;
    std::vector<std::unique_ptr<Player>> _playerList;
    std::size_t _heightMap;
    std::size_t _widthMap;
    std::size_t _newId{0};
    std::vector<Tile> _tiles;
    std::vector<Egg> _vecEggs;

    [[nodiscard]] std::size_t getTileIndex(std::size_t x, std::size_t y) const;
    std::size_t getTileIndex(const Pos& pos) const;
    void erasePlayerFromTile(std::size_t pos1dVec, std::size_t id);
    void eraseEggFromTile(std::size_t pos1dVec, std::size_t id);
    std::optional<Egg> getTeamEgg(const std::string_view& teamName);
};
}  // namespace zappy::server::game
