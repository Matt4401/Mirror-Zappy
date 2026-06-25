/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Pin
*/

#include "Pin.hpp"

#include <cstdint>

#include "Core.hpp"
#include "IGuiCommand.hpp"
#include "game/Player.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::guiCommand {

Pin::Pin(shared::protocol::client::Pin cmd) : _id(cmd.playerId) {}

GuiResponse Pin::execute(Core& core) {
    if (!core.world().playerList().contains(_id)) {
        return GuiResponse{.message = shared::protocol::Emitter::build(shared::protocol::server::Sbp{})};
    }
    const auto& player = core.world().playerList().at(_id);
    const auto [x, y] = player->position();
    const auto inventory = player->inventory();

    const auto payload = shared::protocol::Emitter::build(shared::protocol::server::Pin{
        .playerId = _id,
        .x = static_cast<int>(x),
        .y = static_cast<int>(y),
        .food = static_cast<int>(inventory.at(static_cast<std::uint8_t>(game::ItemType::Food))),
        .linemate = static_cast<int>(inventory.at(static_cast<std::uint8_t>(game::ItemType::Linemate))),
        .deraumere = static_cast<int>(inventory.at(static_cast<std::uint8_t>(game::ItemType::Deraumere))),
        .sibur = static_cast<int>(inventory.at(static_cast<std::uint8_t>(game::ItemType::Sibur))),
        .mendiane = static_cast<int>(inventory.at(static_cast<std::uint8_t>(game::ItemType::Mendiane))),
        .phiras = static_cast<int>(inventory.at(static_cast<std::uint8_t>(game::ItemType::Phiras))),
        .thystame = static_cast<int>(inventory.at(static_cast<std::uint8_t>(game::ItemType::Thystame))),
    });
    return GuiResponse{.message = payload};
}
}  // namespace zappy::server::guiCommand
