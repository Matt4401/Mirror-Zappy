/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Player
*/

#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "command/ICommand.hpp"

namespace zappy::server::game {

class World;

struct Position {
    std::size_t x;
    std::size_t y;
};

enum class ItemType : std::uint8_t { Food, Linemate, Deraumere, Sibur, Mendiane, Phiras, Thystame, COUNT };

using CoordinateMove = std::pair<int, int>;

constexpr std::array<std::string, static_cast<std::size_t>(ItemType::COUNT)> kInventoryOrder = {
    "food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};

enum class cardinalPoint : uint8_t { NORTH, EAST, SOUTH, WEST, COUNT };

constexpr std::array<CoordinateMove, 4> playerMove = {{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};

constexpr std::array<CoordinateMove, 4> diagonalLeftMove = {{{-1, -1}, {1, -1}, {1, 1}, {-1, 1}}};

constexpr std::array<CoordinateMove, 4> leftTile = {{{-1, 0}, {0, -1}, {1, 0}, {0, 1}}};

static constexpr std::size_t kNbLifeTickFood = 126;
static constexpr std::size_t kNbStartFood = 10;
static constexpr std::uint8_t kMaxNbCmd = 10;
static constexpr int kNbLevel = 7;

static std::unordered_map<std::string, ItemType> mapItemString() {
    static const std::unordered_map<std::string, ItemType> kMapItemString = {
        {"linemate", ItemType::Linemate}, {"deraumere", ItemType::Deraumere}, {"sibur", ItemType::Sibur},
        {"mendiane", ItemType::Mendiane}, {"phiras", ItemType::Phiras},       {"thystame", ItemType::Thystame},
        {"food", ItemType::Food},
    };
    return kMapItemString;
}

class Player {
  public:
    Player(const Player& other) = delete;
    Player& operator=(const Player& other) = delete;
    Player(Player&& other) = delete;
    Player& operator=(Player&& other) = delete;

    explicit Player(std::size_t id, std::size_t x, std::size_t y, cardinalPoint orient);
    ~Player() = default;

    void addItem(ItemType item, std::size_t quantity = 1);
    bool subItem(ItemType item, std::size_t quantity = 1);
    void setItem(ItemType item, size_t amount);
    [[nodiscard]] std::size_t getItem(ItemType item) const;

    void pushCommand(std::unique_ptr<command::ICommand> command);
    void update(World& world);
    void moveForward(const Position& limit);
    void moveWithOrientation(const Position& limit, cardinalPoint orientation);

    void addResponse(const std::string&);
    std::vector<std::string> responses();
    [[nodiscard]] Position position() const;
    [[nodiscard]] std::array<std::size_t, static_cast<std::uint8_t>(ItemType::COUNT)> inventory() const;
    std::vector<Position> getLookPos(Position mapLimit);

    void setOrientation(cardinalPoint orient);
    [[nodiscard]] cardinalPoint orientation() const;

    [[nodiscard]] std::size_t id() const;
    [[nodiscard]] std::size_t nbLifeTick() const;
    void kill();
    /*
     * @brief: don't une this function it's for test.
     **/
    void setPosition(Position pos);

    [[nodiscard]] int cmdTick() const;

    [[nodiscard]] bool hasCommands() const;
    void tryStartNextCommand(World& world, bool isMidTick = false);
    [[nodiscard]] int level() const;
    void levelUp();
    [[nodiscard]] bool checkIncantationRequirements(
        const std::array<std::size_t, static_cast<std::uint8_t>(ItemType::COUNT)>& resources) const;
    void setIncanting(bool isIncanting);

    [[nodiscard]] bool isDead() const;

  private:
    std::array<std::size_t, static_cast<std::uint8_t>(ItemType::COUNT)> _inventory{};
    cardinalPoint _orientation;
    std::queue<std::unique_ptr<command::ICommand>> _commands;
    std::unique_ptr<command::ICommand> _currentCommand{nullptr};
    std::size_t _cmdTick{0};
    std::size_t _lifeTick;
    Position _pos{.x = 0, .y = 0};
    std::vector<std::string> _buffersResponses;
    bool _isDead{false};
    std::size_t _id;
    bool _isNewCommand{false};
    int _level{1};
    bool _isIncanting{false};

    Position getNthDiagonalLeftPosition(std::size_t n, Position mapLimit);
    Position getLeftTile(Position ogPos, const Position& mapLimit);
};
}  // namespace zappy::server::game
