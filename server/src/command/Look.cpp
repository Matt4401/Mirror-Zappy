/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Look
*/

#include "Look.hpp"

#include <string>

#include "ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {
Look::Look() : ACommand(kTimeLimit) {}

void Look::execute(game::World& world, game::Player& player) {
    std::string responses{};
    auto diagPos = player.getLookPos(world.sizeMap());
    responses = world.jsp(diagPos);
}
}  // namespace zappy::server::command
