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

#include "Logger.hpp"
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
    auto [x, y] = player.position();
    world.addGuiEvent(shared::protocol::Emitter::build(shared::protocol::server::Pdr{
        .playerId = static_cast<int>(player.id()),
        .resourceId = static_cast<int>(item),
    }));
    world.addItemOnGround(item, player.position());
    const auto inventory = player.inventory();
    player.addResponse("ok\n");
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
    Logger::logInfo("Set command executed for player " + std::to_string(player.id()) + " at position (" +
                    std::to_string(x) + ", " + std::to_string(y) + ") with item " + _arg + ".");
}
}  // namespace zappy::server::command
