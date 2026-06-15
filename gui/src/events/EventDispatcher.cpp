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
            using RealCmdType = std::decay_t<decltype(cmd)>;
            auto type = std::type_index(typeid(RealCmdType));

            if (auto it = _listeners.find(type); it != _listeners.end()) {
                for (const auto& [token, anyCallback] : it->second) {
                    auto callback = std::any_cast<std::function<void(const RealCmdType&)>>(anyCallback);
                    callback(cmd);
                }
            }
        },
        command);
}
}  // namespace zappy::gui::events
