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
#include <vector>

#include "Team.hpp"
#include "command/ICommand.hpp"
#include "game/Player.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server::game {

struct Egg {
    std::size_t id;
    Position position;
    std::string teamName;
};

struct Tile {
    std::vector<std::size_t> players;
    std::vector<std::size_t> eggs;
    std::array<std::size_t, static_cast<uint8_t>(ItemType::COUNT)> resources;
};

// NOLINTNEXTLINE
const std::unordered_map<cardinalPoint, std::string> kCardinalPointToStr = {
    {cardinalPoint::NORTH, "north"},
    {cardinalPoint::EAST, "east"},
    {cardinalPoint::SOUTH, "south"},
    {cardinalPoint::WEST, "west"},
};

const std::unordered_map<ItemType, double> kDensityItem = {
    {ItemType::Food, 0.5},     {ItemType::Linemate, 0.3}, {ItemType::Deraumere, 0.15}, {ItemType::Sibur, 0.1},
    {ItemType::Mendiane, 0.1}, {ItemType::Phiras, 0.08},  {ItemType::Thystame, 0.05},
};

class World {
  public:
    explicit World(const parser::ServerConfig& config);
    void setSpawnEggs(size_t clientLimit, std::string_view teamName);
    void addItemToMap();
    static cardinalPoint randomCardinalPoint();
    ~World() = default;

    World(const World& other) = delete;
    World& operator=(const World& other) = delete;
    World(World&& other) = delete;
    World& operator=(World&& other) = delete;

    Position sizeMap() const;
    [[nodiscard]] std::optional<size_t> spawnPlayer(std::string_view teamName);
    Position getTilePosition(std::size_t position1D) const;

    void update();
    [[nodiscard]] std::unordered_map<std::size_t, std::vector<std::string>> getAllResponsesBuffer() const;
    [[nodiscard]] std::vector<std::string> getAndClearGuiEvents();
    void pushCommandToPlayer(std::size_t playerId, std::unique_ptr<command::ICommand> command) const;
    void removePlayerFromTeam(std::size_t id) const;
    void updatePositionOnMap(std::size_t id, const Position& oldPosition, const Position& newPosition);

    std::size_t removePlayer(std::size_t id);
    std::vector<std::size_t> collectAndKillDeadPlayers() const;
    std::size_t getAvailableSlotInTeam(std::string_view teamName) const;
    void eject(std::size_t id);
    bool hasEjectableTargetOnTile(const Position& position, std::size_t id) const;
    bool isEggOnTile(const Position& position) const;

    const std::unordered_map<std::size_t, std::unique_ptr<Player>>& playerList() const;
    void addItemOnGround(ItemType item, Position pos);
    void removeItemOnGround(ItemType item, Position pos);
    std::array<std::size_t, static_cast<uint8_t>(ItemType::COUNT)> tileResources(Position position) const;

    [[nodiscard]] int getNextExecutionTick() const;

  private:
    std::unordered_map<std::string, std::unique_ptr<Team>> _teamList;
    std::unordered_map<std::size_t, std::unique_ptr<Player>> _playerList;
    std::size_t _heightMap;
    std::size_t _widthMap;
    std::size_t _newId{0};
    std::vector<Tile> _tiles;
    std::unordered_map<std::size_t, Egg> _vecEggs;

    std::vector<std::string> _guiEvents;

    [[nodiscard]] std::size_t getTileIndex(std::size_t x, std::size_t y) const;
    [[nodiscard]] std::size_t getTileIndex(const Position& position) const;
    void erasePlayerFromTile(std::size_t position1dVec, std::size_t id);
    void eraseEggFromTile(std::size_t position1dVec, std::size_t id);
    std::optional<Egg> getTeamEgg(const std::string_view& teamName);
    void removeFromMap(std::size_t id);
};
}  // namespace zappy::server::game
