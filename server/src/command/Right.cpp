/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Right
*/

#include "Right.hpp"

#include "command/ACommand.hpp"
#include "game/Player.hpp"

namespace zappy::server::command {

Right::Right() : ACommand{kTimeLimit} {}

void Right::execute(game::World& /*world*/, game::Player& player) {
    setRequiredTicks(kTimeLimit);

    const auto newOrientation =
        static_cast<game::orientation>((static_cast<std::uint8_t>(player.getOrientation()) + 1) % 4);
    player.setOrientation(newOrientation);
    player.addResponse("ok\n");
}
}  // namespace zappy::server::command
