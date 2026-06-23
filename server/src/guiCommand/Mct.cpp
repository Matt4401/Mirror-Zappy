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
#include "guiCommand/IGuiCommand.hpp"
#include "protocol/Commands.hpp"

namespace zappy::server::guiCommand {

GuiResponse Mct::execute(Core& core) {
    auto worldSize = core.world().sizeMap();
    std::string payload;

    for (std::size_t x = 0; x < worldSize.x; x++) {
        for (std::size_t y = 0; y < worldSize.y; y++) {
            Bct bctCommand(shared::protocol::client::Bct{.x = static_cast<int>(x), .y = static_cast<int>(y)});
            payload.append(bctCommand.execute(core).message);
        }
    }
    return {.message = payload};
}

}  // namespace zappy::server::guiCommand
