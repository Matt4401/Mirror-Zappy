/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Event
*/

#include "Event.hpp"
#include "context/EventContext.hpp"
#include <raylib.h>


namespace zappy::gui::raylib::rcore {
    Event::Event() = default;

    void Event::handleEvent(EventContext& context) {
        for (const auto& [key, func] : _keyEvents) {
            if (isKeyDown(key)) {
                func(context);
            }
        }
    }

    void Event::update() {
        _pressedKeys.clear();

        int key = GetKeyPressed();
        while (key != 0) {
            _pressedKeys.push_back(key);
            key = GetKeyPressed();
        }

        _mousePosition = GetMousePosition();
        _mouseWheelMove = GetMouseWheelMove();
    }
}  // namespace zappy::gui::raylib::rcore
