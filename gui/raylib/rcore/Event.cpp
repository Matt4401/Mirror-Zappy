/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Event
*/

#include "Event.hpp"

#include <raylib.h>

namespace zappy::gui::raylib::rcore {
Event::Event() = default;

void Event::update() {
    _pressedKeys.clear();

    _mousePosition = GetMousePosition();
    _mouseWheelMove = GetMouseWheelMove();
}
}  // namespace zappy::gui::raylib::rcore
