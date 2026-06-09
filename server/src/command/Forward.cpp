/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** forward
*/

#include "Forward.hpp"

#include <iostream>

#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

Forward::Forward() : ACommand{7} {}

void Forward::execute(game::World& /*world*/, game::Player& /*player*/) {
    std::cout << "Executing Forward. Need to be implemented." << std::endl;
};

}  // namespace zappy::server::command
