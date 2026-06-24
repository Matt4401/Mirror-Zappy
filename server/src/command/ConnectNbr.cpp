/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Connect_nbr implementation
*/

#include "ConnectNbr.hpp"

#include <cstddef>
#include <format>

#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

ConnectNbr::ConnectNbr() : ACommand{kTimeLimit} {}

bool ConnectNbr::start(game::World& world, game::Player& player) {
    const auto& teamName = world.getPlayerTeam(player.id());
    const std::size_t availableSlots = world.getAvailableSlotInTeam(teamName);
    player.addResponse(std::format("{}\n", availableSlots));
    return true;
}

void ConnectNbr::execute(game::World& world, game::Player& player) {}

}  // namespace zappy::server::command
