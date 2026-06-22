/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Unkown command
*/

#include "Unknown.hpp"

#include "ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

Unknown::Unknown() : ACommand{kTimeLimit} {}

bool Unknown::start(game::World& /*world*/, game::Player& /*player*/) { return false; }

void Unknown::execute(game::World& /*world*/, game::Player& /*player*/) {}

}  // namespace zappy::server::command
