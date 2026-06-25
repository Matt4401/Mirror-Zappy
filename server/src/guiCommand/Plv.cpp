/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Plv
*/

#include "Plv.hpp"

#include "IGuiCommand.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"
#include "server/src/Core.hpp"

namespace zappy::server::guiCommand {
Plv::Plv(const shared::protocol::client::Plv cmd) : _id(cmd.playerId) {}

GuiResponse Plv::execute(Core& core) {
    GuiResponse response;
    if (!core.world().playerList().contains(_id)) {
        response.message = shared::protocol::Emitter::build(shared::protocol::server::Sbp{});
        return response;
    }
    const auto& player = core.world().playerList().at(_id);
    const auto payload =
        shared::protocol::Emitter::build(shared::protocol::server::Plv{.playerId = _id, .level = player->level()});
    response.message = payload;
    return response;
}
}  // namespace zappy::server::guiCommand
