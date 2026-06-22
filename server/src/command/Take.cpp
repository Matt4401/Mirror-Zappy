/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Take
*/

#include "Take.hpp"

#include <cstdint>
#include <string>
#include <utility>

#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {
Take::Take(std::string arg) : ACommand{kTimeLimit}, _arg(std::move(arg)) {}

bool Take::start(game::World& world, game::Player& player) {
    const auto mapItemString = game::mapItemString();
    const auto it = mapItemString.find(_arg);
    if (it == mapItemString.end()) {
        return false;
    }
    const auto& resources = world.resourcesAt(player.position());
    return resources.at(static_cast<std::uint8_t>(it->second)) > 0;
}
void Take::execute(game::World& world, game::Player& player) {
    const game::ItemType item = game::mapItemString().at(_arg);
    world.removeItemOnGround(item, player.position());
    player.addItem(item);
}
}  // namespace zappy::server::command
