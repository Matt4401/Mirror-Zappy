/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Set
*/

#include "Set.hpp"

#include <cstdint>
#include <string>
#include <utility>

#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::command {

Set::Set(std::string arg) : ACommand{kTimeLimit}, _arg(std::move(arg)) {}

bool Set::start(game::World& /*world*/, game::Player& player) {
    const auto mapItemString = game::mapItemString();
    const auto it = mapItemString.find(_arg);
    if (it == mapItemString.end()) {
        return false;
    }
    const auto inventory = player.inventory();
    return inventory.at(static_cast<std::uint8_t>(it->second)) > 0;
}
void Set::execute(game::World& world, game::Player& player) {
    const game::ItemType item = game::mapItemString().at(_arg);
    if (!player.subItem(item)) {
        player.addResponse("ko\n");
        return;
    }
    world.addItemOnGround(item, player.position());
    world.addGuiEvent(shared::protocol::Emitter::build(shared::protocol::server::Pdr{
        .playerId = static_cast<int>(player.id()),
        .resourceId = static_cast<int>(item),
    }));
    player.addResponse("ok\n");
}
}  // namespace zappy::server::command
