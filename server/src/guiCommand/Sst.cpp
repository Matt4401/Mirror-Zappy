/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** sst
*/

#include "Sst.hpp"

#include <string>

#include "Core.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::guiCommand {

Sst::Sst(int timeUnit) : _timeUnit(timeUnit) {}

std::string Sst::execute(Core& core) {
    const auto timeUnit = core.config().freq;

    core.config().freq = _timeUnit;
    return shared::protocol::Emitter::build(shared::protocol::server::Sst{.timeUnit = _timeUnit});
}
}  // namespace zappy::server::guiCommand
