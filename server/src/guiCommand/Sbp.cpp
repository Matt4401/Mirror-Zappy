/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** sbp
*/

#include "Sbp.hpp"

#include "guiCommand/IGuiCommand.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::guiCommand {

GuiResponse Sbp::execute(Core& /*core*/) {
    GuiResponse response;

    response.message = shared::protocol::Emitter::build(shared::protocol::server::Sbp{});
    return response;
}

}  // namespace zappy::server::guiCommand
