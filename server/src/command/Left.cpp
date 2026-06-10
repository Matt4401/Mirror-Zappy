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

    const auto currentOrientation = static_cast<std::uint8_t>(player.orientation());
    constexpr auto nbCardinalPoint = static_cast<std::uint8_t>(game::cardinalPoint::COUNT);
    const auto newOrientation =
        static_cast<game::cardinalPoint>((currentOrientation + (nbCardinalPoint - 1)) % nbCardinalPoint);
    player.setOrientation(newOrientation);
    player.addResponse("ok\n");
}
}  // namespace zappy::server::command
