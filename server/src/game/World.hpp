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

struct Condition {
    std::size_t nbPlayer;
    std::array<std::size_t, static_cast<std::uint8_t>(game::ItemType::COUNT)> resources;
};

class World {
  public:
    explicit World(const parser::ServerConfig& config);
    void setSpawnEggs(size_t clientLimit, std::string_view teamName);
    /*
     * @brief: this function spawns the resources on the map.
     **/
    void addItemsToMap();
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
    void pushCommandToPlayer(std::size_t playerId, std::unique_ptr<command::ICommand> command);
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
    bool removeItemOnGround(ItemType item, Position pos);

    [[nodiscard]] int getNextExecutionTick() const;
    [[nodiscard]] std::array<std::size_t, static_cast<uint8_t>(ItemType::COUNT)> resourcesAt(Position pos) const;
    [[nodiscard]] std::string getPlayerTeam(std::size_t id) const;

    void addGuiEvent(const std::string& event);
    std::string visionOfPlayer(const std::vector<Position>& Positions) const;
    void clearAllResourcesAndEggs();

    void layEgg(const Player& player);
    const std::unordered_map<std::size_t, Egg>& vecEggs() const;
    std::vector<int> playersWithSameLevelOnTile(Position position, int level) const;

  private:
    std::unordered_map<std::string, std::unique_ptr<Team>> _teamList;
    std::unordered_map<std::size_t, std::unique_ptr<Player>> _playerList;
    std::size_t _heightMap;
    std::size_t _widthMap;
    std::size_t _newId{0};
    std::vector<Tile> _tiles;
    std::unordered_map<std::size_t, Egg> _vecEggs;
    std::size_t respawnTicks{0};

    std::vector<std::string> _guiEvents;

    [[nodiscard]] std::size_t getTileIndex(std::size_t x, std::size_t y) const;
    [[nodiscard]] std::size_t getTileIndex(const Position& position) const;
    void erasePlayerFromTile(std::size_t position1dVec, std::size_t id);
    void eraseEggFromTile(std::size_t position1dVec, std::size_t id);
    std::optional<Egg> getTeamEgg(const std::string_view& teamName);
    void removeFromMap(std::size_t id);
    [[nodiscard]] static std::unordered_map<ItemType, double> densityItem();
    [[nodiscard]] static std::unordered_map<cardinalPoint, std::string> cardinalPointToStr();

    static std::string resourcesName(ItemType item);
    static std::string transformResourcesToStr(const Tile& tile);

    static constexpr std::size_t kNbTicksToRespawn = 20;
};
}  // namespace zappy::server::game
