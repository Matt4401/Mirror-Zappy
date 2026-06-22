/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** EventDispatcher
*/

#include "EventDispatcher.hpp"

#include <variant>

#include "protocol/Commands.hpp"

namespace zappy::gui::events {
void EventDispatcher::dispatch(const shared::protocol::ServerCommand& command) {
    std::visit([this](const auto& event) { dispatch(event); }, command);
}

}  // namespace zappy::gui::events
