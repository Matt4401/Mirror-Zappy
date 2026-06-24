/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Right
*/

#include "Right.hpp"

#include <cstdint>

#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::command {

Right::Right() : ACommand{kTimeLimit} {}

void Right::execute(game::World& world, game::Player& player) {
    setRequiredTicks(kTimeLimit);

    constexpr auto nbCardinalPoint = static_cast<std::uint8_t>(game::cardinalPoint::COUNT);
    const auto newOrientation =
        static_cast<game::cardinalPoint>((static_cast<std::uint8_t>(player.orientation()) + 1) % nbCardinalPoint);
    player.setOrientation(newOrientation);
    player.addResponse("ok\n");
    world.addGuiEvent(shared::protocol::Emitter::build(shared::protocol::server::Ppo{
        .playerId = static_cast<int>(player.id()),
        .x = static_cast<int>(player.position().x),
        .y = static_cast<int>(player.position().y),
        .orientation = static_cast<int>(newOrientation),
    }));
}
}  // namespace zappy::server::command
