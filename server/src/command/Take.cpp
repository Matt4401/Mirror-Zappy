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
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

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
    auto [x, y] = player.position();
    if (!world.removeItemOnGround(item, player.position())) {
        player.addResponse("ko\n");
        return;
    }
    player.addItem(item);
    world.addGuiEvent(shared::protocol::Emitter::build(shared::protocol::server::Pgt{
        .playerId = static_cast<int>(player.id()),
        .resourceId = static_cast<int>(item),
    }));
    const auto inventory = player.inventory();
    world.addGuiEvent(shared::protocol::Emitter::build(shared::protocol::server::Pin{
        .playerId = static_cast<int>(player.id()),
        .x = static_cast<int>(x),
        .y = static_cast<int>(y),
        .food = static_cast<int>(inventory.at(static_cast<std::uint8_t>(game::ItemType::Food))),
        .linemate = static_cast<int>(inventory.at(static_cast<std::uint8_t>(game::ItemType::Linemate))),
        .deraumere = static_cast<int>(inventory.at(static_cast<std::uint8_t>(game::ItemType::Deraumere))),
        .sibur = static_cast<int>(inventory.at(static_cast<std::uint8_t>(game::ItemType::Sibur))),
        .mendiane = static_cast<int>(inventory.at(static_cast<std::uint8_t>(game::ItemType::Mendiane))),
        .phiras = static_cast<int>(inventory.at(static_cast<std::uint8_t>(game::ItemType::Phiras))),
        .thystame = static_cast<int>(inventory.at(static_cast<std::uint8_t>(game::ItemType::Thystame))),
    }));
    player.addResponse("ok\n");
}
}  // namespace zappy::server::command
