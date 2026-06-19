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

namespace zappy::server::command {

Set::Set(std::string arg) : ACommand{kTimeLimit}, _arg(std::move(arg)) {}

bool Set::start(game::World& /*world*/, game::Player& player) {
    const auto it = game::kMapItemString.find(_arg);
    if (it == game::kMapItemString.end()) {
        return false;
    }
    const auto inventory = player.inventory();

    return inventory.at(static_cast<std::uint8_t>(it->second)) > 0;
}
void Set::execute(game::World& world, game::Player& player) {
    const game::ItemType item = game::kMapItemString.at(_arg);
    player.subItem(item);
    world.addItemOnGround(item, player.position());
}
}  // namespace zappy::server::command
