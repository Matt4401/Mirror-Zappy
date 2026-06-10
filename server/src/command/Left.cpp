/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Left
*/

#include "Left.hpp"

#include <cstdint>

#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

Left::Left() : ACommand{kTimeLimit} {}

void Left::execute(game::World& /*world*/, game::Player& player) {
    setRequiredTicks(kTimeLimit);

    auto newOrientation = static_cast<std::uint8_t>(player.getOrientation()) - 1;
    if (newOrientation < 0) {
        newOrientation = 3;
    }
    player.setOrientation(static_cast<game::orientation>(newOrientation));
    player.addResponse("ok\n");
}
}  // namespace zappy::server::command
