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

void Forward::execute(game::World& /*world*/, game::Player& player) {
    const auto limitMap = game::World::limitMap();

    player.moveUp(limitMap);
    player.addResponse("ok\n");
};

}  // namespace zappy::server::command
