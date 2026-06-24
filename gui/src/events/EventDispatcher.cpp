/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** EventDispatcher
*/

#include "EventDispatcher.hpp"

#include <typeindex>
#include <variant>

#include "protocol/Commands.hpp"

namespace zappy::gui::events {
void EventDispatcher::dispatch(const shared::protocol::ServerCommand& command) {
    std::visit([this](const auto& event) { dispatch(event); }, command);

    const auto type = std::type_index(typeid(shared::protocol::ServerCommand));
    if (auto it = _listeners.find(type); it != _listeners.end()) {
        for (auto& listener : it->second) {
            listener.callback(&command);
        }
    }
}

}  // namespace zappy::gui::events
