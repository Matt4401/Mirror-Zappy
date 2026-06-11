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
#include "util/DataStructures.hpp"

namespace zappy::server::game {

class Player;

enum class ItemType : uint8_t { Food, Linemate, Deraumere, Sibur, Mendiane, Phiras, Thystame, COUNT };

enum class cardinalPoint : uint8_t { NORTH, EAST, SOUTH, WEST, COUNT };

struct Pos {
    std::size_t x;
    std::size_t y;
};

struct Tile {
    bool isEgg;
    std::vector<std::size_t> players;
    std::array<std::size_t, static_cast<uint8_t>(ItemType::COUNT)> ressources;
};

class World {
  public:
    static std::pair<std::size_t, std::size_t> getRandomPlace(std::size_t heightMap, std::size_t widthMap);
    void setSpawnEggs();
    static cardinalPoint randomCardinalPoint();
    explicit World(const util::Config& config);
    ~World() = default;

    World(const World& other) = delete;
    World& operator=(const World& other) = delete;
    World(World&& other) = delete;
    World& operator=(World&& other) = delete;

    std::pair<std::size_t, std::size_t> limitMap();
    [[nodiscard]] std::optional<size_t> spawnPlayer(std::string_view teamName);

    void update();
    void removeFromMap(std::size_t id);
    std::optional<std::size_t> Kill(std::size_t id);
    std::vector<std::size_t> removeDead();
    void pushCommandToPlayer(std::size_t playerId, std::unique_ptr<command::ICommand> command) const;
    void removePlayerFromTeam(std::size_t id) const;
    void updatePosOnMap(std::size_t id, const Pos& oldPos, const Pos& newPos);

  private:
    std::unordered_map<std::string_view, std::unique_ptr<Team>> _teamList;
    std::vector<std::unique_ptr<Player>> _playerList;
    std::size_t _heightMap;
    std::size_t _widthMap;
    std::size_t _newId{0};
    std::vector<Tile> _tiles;
    std::vector<std::size_t> _vecEggs;
    std::size_t _clientLimit;

    [[nodiscard]] std::size_t getTileIndex(int x, int y) const;
    std::size_t getTileIndex(const Pos& pos) const;
    void erasePlayerFromTiles(std::size_t pos1dVec, std::size_t id);
};
}  // namespace zappy::server::game
