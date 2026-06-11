/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** forward
*/

#include "Forward.hpp"

#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

Forward::Forward() : ACommand{kTimeLimit} {}

void Forward::execute(game::World& world, game::Player& player) {
    const auto [limitX, limitY] = world.limitMap();

    const auto [oldX, oldY] = player.position();
    player.moveUp({.x = limitX, .y = limitY});
    const auto [newX, newY] = player.position();
    world.updatePosOnMap(player.id(), {.x = oldX, .y = oldY}, {.x = newX, .y = newY});
    player.addResponse("ok\n");
};

}  // namespace zappy::server::command
