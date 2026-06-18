/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Sgt
*/

#include "Sgt.hpp"

#include <string>

#include "Core.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::guiCommand {

std::string Sgt::execute(Core& core) {
    const auto freq = core.config().freq;
    auto payload = shared::protocol::Emitter::build(shared::protocol::server::Sgt{.timeUnit = static_cast<int>(freq)});

    return payload;
}

}  // namespace zappy::server::guiCommand
