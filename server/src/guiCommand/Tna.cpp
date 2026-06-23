/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Tna
*/

#include "Tna.hpp"

#include "Core.hpp"
#include "guiCommand/IGuiCommand.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::guiCommand {

GuiResponse Tna::execute(Core& core) {
    GuiResponse response{};

    for (const auto& teamName : core.config().teamNames) {
        response.message.append(shared::protocol::Emitter::build(shared::protocol::server::Tna{.teamName = teamName}));
    }
    return response;
}

}  // namespace zappy::server::guiCommand
