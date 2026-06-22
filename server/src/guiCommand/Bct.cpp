/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** bct
*/

#include "Bct.hpp"

#include <cstddef>
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

    if (_x < 0 || static_cast<std::uint32_t>(_x) >= mapSize.x || _y < 0 ||
        static_cast<std::uint32_t>(_y) >= mapSize.y) {
        return "sbp\n";
    }
    auto resources =
        core.world().resourcesAt(game::Position{.x = static_cast<std::size_t>(_x), .y = static_cast<std::size_t>(_y)});
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
