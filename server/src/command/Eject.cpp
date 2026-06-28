/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Eject
*/

#include "Eject.hpp"

#include "ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

Eject::Eject() : ACommand{kTimeLimit} {}

bool Eject::start(game::World& /*world*/, game::Player& /*player*/) { return true; }

void Eject::execute(game::World& world, game::Player& player) {
    world.eject(player.id());
    player.addResponse("ok\n");
}

}  // namespace zappy::server::command
