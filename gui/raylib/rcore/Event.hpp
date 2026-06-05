/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Event
*/

#pragma once
#include <raylib.h>

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

    static bool isKeyPressed(int key) { return IsKeyPressed(key); }
    static bool isKeyDown(int key) { return IsKeyDown(key); }
    static bool isKeyReleased(int key) { return IsKeyReleased(key); }

    static bool isMouseButtonPressed(int button) { return IsMouseButtonPressed(button); }
    static bool isMouseButtonDown(int button) { return IsMouseButtonDown(button); }
    static bool isMouseButtonReleased(int button) { return IsMouseButtonReleased(button); }

    static rmath::Vector2 getMousePosition() { return GetMousePosition(); }
    static double getMouseWheelMove() { return GetMouseWheelMove(); }

  protected:
  private:
};
}  // namespace zappy::gui::raylib::rcore
