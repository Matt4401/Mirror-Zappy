/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** broadcast command
*/

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>

#include "ACommand.hpp"
#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

class Broadcast : public ACommand {
  public:
    explicit Broadcast(std::string args);
    ~Broadcast() override = default;
    Broadcast(const Broadcast& other) = delete;
    Broadcast& operator=(const Broadcast& other) = delete;
    Broadcast(Broadcast&& other) = delete;
    Broadcast& operator=(Broadcast&& other) = delete;

    void execute(game::World& world, game::Player& player) override;

  private:
    using Position = std::pair<int, int>;

    static std::uint8_t getDirection(game::Player& player, game::Player& otherPlayer);
    static const std::unordered_map<std::uint8_t, Position>& directionMap();

    std::string _args;
    static constexpr int kTimeLimit = 7;
};

}  // namespace zappy::server::command
