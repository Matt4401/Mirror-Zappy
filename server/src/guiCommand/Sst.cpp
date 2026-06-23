/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** sst
*/

#include "Sst.hpp"

#include "Core.hpp"
#include "guiCommand/IGuiCommand.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::guiCommand {

Sst::Sst(int timeUnit) : _timeUnit(timeUnit) {}

GuiResponse Sst::execute(Core& core) {
    if (_timeUnit <= 0) {
        return {.message = shared::protocol::Emitter::build(shared::protocol::server::Sbp{}), .isArgumentError = true};
    }
    core.config().freq = _timeUnit;
    return {.message = shared::protocol::Emitter::build(shared::protocol::server::Sst{.timeUnit = _timeUnit}),
            .sendToEveryone = true};
}
}  // namespace zappy::server::guiCommand
