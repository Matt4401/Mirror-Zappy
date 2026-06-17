/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Tna
*/

#include "Tna.hpp"

#include <string>

#include "Core.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::guiCommand {

std::string Tna::execute(Core& core) {
    std::string response{};

    for (const auto& teamName : core.config().teamNames) {
        response.append(shared::protocol::Emitter::build(shared::protocol::server::Tna{.teamName = teamName}));
    }
    return response;
}

}  // namespace zappy::server::guiCommand
