/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Color
*/

#pragma once
#include <raylib.h>
#include <sys/types.h>

#include <cstdint>

namespace zappy::gui::raylib {
class Color {
  public:
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : _color(r, g, b, a) {}
    Color(::Color color) : _color{color} {}
    ~Color() = default;
    Color(const Color& other) = default;
    Color& operator=(const Color& other) = default;
    Color(Color&& other) noexcept : _color{other._color} {}
    Color& operator=(Color&& other) noexcept {
        if (this != &other) {
            _color = other._color;
        }
        return *this;
    }

    [[nodiscard]] ::Color color() const { return _color; }

    [[nodiscard]] uint8_t r() const { return _color.r; }
    [[nodiscard]] uint8_t g() const { return _color.g; }
    [[nodiscard]] uint8_t b() const { return _color.b; }
    [[nodiscard]] uint8_t a() const { return _color.a; }

  protected:
  private:
    ::Color _color;
};
}  // namespace zappy::gui::raylib
