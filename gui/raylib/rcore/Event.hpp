/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Event
*/

#pragma once
#include <raylib.h>

#include <vector>

#include "rcore/Camera.hpp"
#include "rcore/Ray.hpp"
#include "rmath/Vector2.hpp"

namespace zappy::gui::raylib::rcore {
class Event {
  public:
    Event();
    ~Event() = default;
    Event(const Event& other) = delete;
    Event& operator=(const Event& other) = delete;
    Event(Event&& other) = delete;
    Event& operator=(Event&& other) = delete;

    void update();

    [[nodiscard]] static bool isKeyPressed(int key) { return IsKeyPressed(key); }
    [[nodiscard]] static bool isKeyDown(int key) { return IsKeyDown(key); }
    [[nodiscard]] static bool isKeyReleased(int key) { return IsKeyReleased(key); }
    [[nodiscard]] static int getCharPressed() { return GetCharPressed(); }

    [[nodiscard]] rmath::Vector2 getMousePosition() const { return _mousePosition; }
    [[nodiscard]] float getMouseWheelMove() const { return _mouseWheelMove; }

    [[nodiscard]] static bool isMouseButtonDown(int button) { return IsMouseButtonDown(button); }
    [[nodiscard]] static bool isMouseButtonPressed(int button) { return IsMouseButtonPressed(button); }
    [[nodiscard]] static bool isMouseButtonReleased(int button) { return IsMouseButtonReleased(button); }
    [[nodiscard]] static rmath::Vector2 getMousePositionStatic() {
        return {GetMousePosition().x, GetMousePosition().y};
    }
    [[nodiscard]] static float getMouseWheelMoveStatic() { return GetMouseWheelMove(); }
    [[nodiscard]] static Ray mouseRay(const Camera& camera) {
        return Ray(GetScreenToWorldRay(GetMousePosition(), camera.camera()));
    }

  protected:
  private:
    std::vector<int> _pressedKeys;
    rmath::Vector2 _mousePosition;
    float _mouseWheelMove = 0.0F;
};
}  // namespace zappy::gui::raylib::rcore
