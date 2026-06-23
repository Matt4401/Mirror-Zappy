/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** broadcast command
*/

#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <utility>

#include "ACommand.hpp"
#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

static constexpr int kTimeLimit = 7;
static constexpr double kSectorSize = 360 / 8.0;
static constexpr double kHalfSectorSize = kSectorSize / 2.0;
static constexpr std::array<std::uint8_t, 8> directionMapping = {1, 2, 3, 4, 5, 6, 7, 8};

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
    template <typename T>
    using Position = std::pair<T, T>;

    static std::uint8_t getDirection(const game::Player& player, const game::Player& otherPlayer,
                                     const game::World& world);
    static std::uint8_t getDirectionFromAngle(double angleInDegrees);

    std::string _args;
};

}  // namespace zappy::server::command
