/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** sbp
*/

#include "Ppo.hpp"

#include "Core.hpp"
#include "guiCommand/IGuiCommand.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::guiCommand {

Ppo::Ppo(shared::protocol::client::Ppo cmd) : _id(cmd.playerId) {}

GuiResponse Ppo::execute(Core& core) {
    if (!core.world().playerList().contains(_id)) {
        return GuiResponse{.message = shared::protocol::Emitter::build(shared::protocol::server::Sbp{})};
    }
    const auto& player = core.world().playerList().at(_id);
    GuiResponse response;

    response.message = shared::protocol::Emitter::build(
        shared::protocol::server::Ppo{.playerId = static_cast<int>(_id),
                                      .x = static_cast<int>(player->position().x),
                                      .y = static_cast<int>(player->position().y),
                                      .orientation = static_cast<int>(player->orientation())});
    return response;
}

}  // namespace zappy::server::guiCommand
