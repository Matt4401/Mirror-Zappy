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
#include <utility>
#include <vector>

#include "command/ICommand.hpp"
#include "game/World.hpp"

namespace zappy::server::game {

enum class orientation : uint8_t { NORTH = 0, EAST, SOUTH, WEST };

constexpr std::array<std::pair<int, int>, 4> playerMove = {{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};

enum class ItemType : uint8_t { Food, Linemate, Deraumere, Sibur, Mendiane, Phiras, Thystame, COUNT };

class Player {
  public:
    Player(const Player& other) = delete;
    Player& operator=(const Player& other) = delete;
    Player(Player&& other) = delete;
    Player& operator=(Player&& other) = delete;

    explicit Player(int id, std::size_t x, std::size_t y);
    ~Player() = default;

    void addItem(ItemType item, std::size_t quantity = 1);
    void subItem(ItemType item, std::size_t quantity = 1);
    void setItem(ItemType item, size_t amount);
    [[nodiscard]] std::size_t getItem(ItemType item) const;

    void pushCommand(std::unique_ptr<command::ICommand> command);
    void update(World& world);
    void moveUp(const std::pair<std::size_t, std::size_t>& limit);

    void addResponse(const std::string&);
    std::vector<std::string> getResponses();

  private:
    std::array<std::size_t, static_cast<uint8_t>(ItemType::COUNT)> _inventory{};
    orientation _orientation;
    std::queue<std::unique_ptr<command::ICommand>> _commands;
    std::unique_ptr<command::ICommand> _currentCommand{nullptr};
    std::size_t _cmdTick{0};
    std::size_t _lifeTick{0};
    std::pair<std::size_t, std::size_t> _pos{0, 0};
    std::vector<std::string> _buffersResponses;
    int _id;
};
}  // namespace zappy::server::game
