/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Broadcast
*/

#include "Broadcast.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>
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
        std::uint8_t direction = getDirection(player, *otherPlayer);
        if (otherPlayer->id() != player.id()) {
            otherPlayer->addResponse("message " + std::to_string(direction) + ", " + _args + "\n");
        }
    }
}

std::uint8_t Broadcast::getDirection(game::Player& player, game::Player& otherPlayer) {
    const auto [playerX, playerY] = player.position();
    const auto [otherX, otherY] = otherPlayer.position();
    const auto& directionMap = Broadcast::directionMap();

    for (const auto& [direction, position] : directionMap) {
        if (playerX + position.first == otherX && playerY + position.second == otherY) {
            return direction;
        }
    }
    return 0;
}

const std::unordered_map<std::uint8_t, Broadcast::Position>& Broadcast::directionMap() {
    static const std::unordered_map<std::uint8_t, Broadcast::Position> directionMap = {
        {1, {0, -1}}, {2, {-1, -1}}, {3, {-1, 0}}, {4, {-1, 1}}, {5, {0, 1}}, {6, {1, 1}}, {7, {1, 0}}, {8, {1, -1}}};
    return directionMap;
}
}  // namespace zappy::server::command
