/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** mct
*/

#include "Mct.hpp"

#include <cstddef>
#include <string>

#include "Core.hpp"
#include "guiCommand/Bct.hpp"

namespace zappy::server::guiCommand {

std::string Mct::execute(Core& core) {
    auto worldSize = core.world().sizeMap();
    std::string payload;

    for (std::size_t x = 0; x < worldSize.x; x++) {
        for (std::size_t y = 0; y < worldSize.y; y++) {
            Bct bctCommand(static_cast<int>(x), static_cast<int>(y));
            payload.append(bctCommand.execute(core));
        }
    }
    return payload;
}

}  // namespace zappy::server::guiCommand
