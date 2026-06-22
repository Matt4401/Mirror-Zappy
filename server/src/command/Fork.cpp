/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Fork
*/

#include "Fork.hpp"

#include <string>

#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::command {

Fork::Fork() : ACommand{kTimeLimit} {}

bool Fork::start(game::World& world, game::Player& player) {
    world.addGuiEvent(shared::protocol::Emitter::build(shared::protocol::server::Pfk{static_cast<int>(player.id())}));
    return true;
}

void Fork::execute(game::World& world, game::Player& player) {
    world.layEgg(player);
    player.addResponse("ok\n");
}
}  // namespace zappy::server::command
