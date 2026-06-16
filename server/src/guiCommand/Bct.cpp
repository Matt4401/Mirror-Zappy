/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** bct
*/

#include "Bct.hpp"

#include <cstdint>
#include <string>

#include "Core.hpp"
#include "game/Player.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::guiCommand {

Bct::Bct(int x, int y) : _x(x), _y(y) {}

std::string Bct::execute(Core& core) {
    const auto mapSize = core.world().sizeMap();
    auto resources = core.world().getResourcesAt(_x, _y);
    auto payload = shared::protocol::Emitter::build(shared::protocol::server::Bct{
        .x = _x,
        .y = _y,
        .food = static_cast<int>(resources.at(static_cast<std::uint8_t>(game::ItemType::Food))),
        .linemate = static_cast<int>(resources.at(static_cast<std::uint8_t>(game::ItemType::Linemate))),
        .deraumere = static_cast<int>(resources.at(static_cast<std::uint8_t>(game::ItemType::Deraumere))),
        .sibur = static_cast<int>(resources.at(static_cast<std::uint8_t>(game::ItemType::Sibur))),
        .mendiane = static_cast<int>(resources.at(static_cast<std::uint8_t>(game::ItemType::Mendiane))),
        .phiras = static_cast<int>(resources.at(static_cast<std::uint8_t>(game::ItemType::Phiras))),
        .thystame = static_cast<int>(resources.at(static_cast<std::uint8_t>(game::ItemType::Thystame))),
    });

    return payload;
}

}  // namespace zappy::server::guiCommand
