/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** msz
*/

#include "Msz.hpp"

#include "Core.hpp"
#include "guiCommand/IGuiCommand.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::guiCommand {

GuiResponse Msz::execute(Core& core) {
    const auto mapSize = core.world().sizeMap();
    auto payload = shared::protocol::Emitter::build(
        shared::protocol::server::Msz{.width = static_cast<int>(mapSize.x), .height = static_cast<int>(mapSize.y)});

    return {.message = payload};
}

}  // namespace zappy::server::guiCommand
