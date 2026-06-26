/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Look
*/

#include "Look.hpp"

#include <string>

#include "ACommand.hpp"
#include "Logger.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {
Look::Look() : ACommand(kTimeLimit) {}

void Look::execute(game::World& world, game::Player& player) {
    std::string responses{};
    auto diagPos = player.getLookPos(world.sizeMap());
    responses = world.visionOfPlayer(diagPos);
    player.addResponse(responses);
    Logger::logInfo("Look command executed for player " + std::to_string(player.id()) + " with response: " + responses);
}
}  // namespace zappy::server::command
