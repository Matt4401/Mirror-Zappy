/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Broadcast
*/

#include "Broadcast.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <numbers>
#include <string>
#include <utility>

#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

Broadcast::Broadcast(std::string args) : ACommand(kTimeLimit), _args{std::move(args)} {}

void Broadcast::execute(game::World& world, game::Player& player) {
    player.addResponse("ok\n");
    const auto& players = world.playerList();

    for (const auto& [id, otherPlayer] : players) {
        std::uint8_t direction = getDirection(player, *otherPlayer, world);
        if (otherPlayer->id() != player.id()) {
            otherPlayer->addResponse("message " + std::to_string(direction) + ", " + _args + "\n");
        }
    }
}

std::uint8_t Broadcast::getDirection(const game::Player& player, const game::Player& otherPlayer,
                                     const game::World& world) {
    const auto [playerX, playerY] = player.position();
    const auto [otherX, otherY] = otherPlayer.position();
    const auto map = world.sizeMap();
    const Position<int> broadcastArea{static_cast<int>(map.x / 2), static_cast<int>(map.y / 2)};
    Position<int> vector{static_cast<int>(playerX) - static_cast<int>(otherX),
                         static_cast<int>(playerY) - static_cast<int>(otherY)};

    if (vector.first == 0 && vector.second == 0) {
        return 0;
    }
    if (vector.first > broadcastArea.first) {
        vector.first -= static_cast<int>(map.x);
    } else if (vector.first < -broadcastArea.first) {
        vector.first += static_cast<int>(map.x);
    }
    if (vector.second > broadcastArea.second) {
        vector.second -= static_cast<int>(map.y);
    } else if (vector.second < -broadcastArea.second) {
        vector.second += static_cast<int>(map.y);
    }

    const double angleInDegrees = (std::atan2(-vector.second, vector.first) * (180.0 / std::numbers::pi)) - 90.0;

    switch (otherPlayer.orientation()) {
        case game::cardinalPoint::NORTH:
            return getDirectionFromAngle(angleInDegrees);
        case game::cardinalPoint::WEST:
            return getDirectionFromAngle(angleInDegrees - 90.0);
        case game::cardinalPoint::SOUTH:
            return getDirectionFromAngle(angleInDegrees - 180.0);
        case game::cardinalPoint::EAST:
            return getDirectionFromAngle(angleInDegrees - 270.0);
        default:
            return 0;
    }
}

std::uint8_t Broadcast::getDirectionFromAngle(double angleInDegrees) {
    angleInDegrees = std::fmod(angleInDegrees, 360.0);
    if (angleInDegrees < 0.0) {
        angleInDegrees += 360.0;
    }
    const auto sector = static_cast<int>(std::round(angleInDegrees / kSectorSize)) % 8;

    return directionMapping.at(sector);
}

}  // namespace zappy::server::command
