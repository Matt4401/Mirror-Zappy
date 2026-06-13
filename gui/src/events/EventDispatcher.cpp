/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** EventDispatcher
*/

#include "EventDispatcher.hpp"

#include <functional>
#include <type_traits>
#include <typeindex>
#include <variant>

#include "protocol/Commands.hpp"

namespace zappy::gui::events {
void EventDispatcher::dispatch(const shared::protocol::ServerCommand& command) {
    std::visit(
        [this](const auto& cmd) {
            using CmdType = std::decay_t<decltype(cmd)>;
            auto type = std::type_index(typeid(CmdType));
            if (auto it = _listeners.find(type); it != _listeners.end()) {
                for (const auto& [token, callbackAny] : it->second) {
                    if (auto callback = std::any_cast<std::function<void(const CmdType&)>>(&callbackAny)) {
                        (*callback)(cmd);
                    }
                }
            }
        },
        command);
}
}  // namespace zappy::gui::events
