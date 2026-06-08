/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Event
*/

#pragma once
#include <raylib.h>

#include <algorithm>
#include <vector>

#include "rmath/Vector2.hpp"

namespace zappy::gui::raylib::rcore {
class Event {
  public:
    Event() = default;
    ~Event() = default;
    Event(const Event& other) = delete;
    Event& operator=(const Event& other) = delete;
    Event(Event&& other) = delete;
    Event& operator=(Event&& other) = delete;

    void update() {
        _pressedKeys.clear();

        int key = GetKeyPressed();
        while (key != 0) {
            _pressedKeys.push_back(key);
            key = GetKeyPressed();
        }

        _mousePosition = GetMousePosition();
        _mouseWheelMove = GetMouseWheelMove();
    }

    [[nodiscard]] bool isKeyPressed(int key) const {
        return std::ranges::find(_pressedKeys, key) != _pressedKeys.end();
    }

    [[nodiscard]] rmath::Vector2 getMousePosition() const { return _mousePosition; }
    [[nodiscard]] float getMouseWheelMove() const { return _mouseWheelMove; }

  protected:
  private:
    std::vector<int> _pressedKeys;
    rmath::Vector2 _mousePosition;
    float _mouseWheelMove = 0.0F;
};
}  // namespace zappy::gui::raylib::rcore
