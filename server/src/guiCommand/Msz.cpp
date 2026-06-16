/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** msz
*/

#include "Msz.hpp"

#include <string>

#include "Core.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::guiCommand {

std::string Msz::execute(Core& core) {
    const auto mapSize = core.world().sizeMap();
    auto payload = shared::protocol::Emitter::build(
        shared::protocol::server::Msz{.width = static_cast<int>(mapSize.x), .height = static_cast<int>(mapSize.y)});

    return payload;
}

}  // namespace zappy::server::guiCommand
