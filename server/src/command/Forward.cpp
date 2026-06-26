/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** forward
*/

#include "Forward.hpp"

#include <string>

#include "Logger.hpp"
#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

Forward::Forward() : ACommand{kTimeLimit} {}

void Forward::execute(game::World& world, game::Player& player) {
    const auto [limitX, limitY] = world.sizeMap();

    const auto [oldX, oldY] = player.position();
    player.moveForward({.x = limitX, .y = limitY});
    const auto [newX, newY] = player.position();
    world.updatePositionOnMap(player.id(), {.x = oldX, .y = oldY}, {.x = newX, .y = newY});
    player.addResponse("ok\n");
    Logger::logInfo("Forward command executed for player " + std::to_string(player.id()) + " from position (" +
                    std::to_string(oldX) + ", " + std::to_string(oldY) + ") to position (" + std::to_string(newX) +
                    ", " + std::to_string(newY) + ").");
};

}  // namespace zappy::server::command
