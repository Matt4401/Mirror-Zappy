/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Color
*/

#pragma once
#include <raylib.h>

#include <cstdint>

namespace zappy::gui::raylib {
class Color {
  public:
    constexpr Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255) : _color(r, g, b, a) {}
    constexpr Color(::Color color) : _color{color} {}
    ~Color() = default;
    Color(const Color& other) = default;
    Color& operator=(const Color& other) = default;
    Color(Color&& other) noexcept = default;
    Color& operator=(Color&& other) noexcept = default;

    static constexpr Color LightGray() { return {200, 200, 200, 255}; }
    static constexpr Color Gray() { return {130, 130, 130, 255}; }
    static constexpr Color DarkGray() { return {80, 80, 80, 255}; }
    static constexpr Color Yellow() { return {253, 249, 0, 255}; }
    static constexpr Color Gold() { return {255, 203, 0, 255}; }
    static constexpr Color Orange() { return {255, 161, 0, 255}; }
    static constexpr Color Pink() { return {255, 109, 194, 255}; }
    static constexpr Color Red() { return {230, 41, 55, 255}; }
    static constexpr Color Maroon() { return {190, 33, 55, 255}; }
    static constexpr Color Green() { return {0, 228, 48, 255}; }
    static constexpr Color Lime() { return {0, 158, 47, 255}; }
    static constexpr Color DarkGreen() { return {0, 117, 44, 255}; }
    static constexpr Color SkyBlue() { return {102, 191, 255, 255}; }
    static constexpr Color Blue() { return {0, 121, 241, 255}; }
    static constexpr Color DarkBlue() { return {0, 82, 172, 255}; }
    static constexpr Color Purple() { return {200, 122, 255, 255}; }
    static constexpr Color Violet() { return {135, 60, 190, 255}; }
    static constexpr Color DarkPurple() { return {112, 31, 126, 255}; }
    static constexpr Color Beige() { return {211, 176, 131, 255}; }
    static constexpr Color Brown() { return {127, 106, 79, 255}; }
    static constexpr Color DarkBrown() { return {76, 63, 47, 255}; }
    static constexpr Color White() { return {255, 255, 255, 255}; }
    static constexpr Color Black() { return {0, 0, 0, 255}; }
    static constexpr Color Blank() { return {0, 0, 0, 0}; }
    static constexpr Color Magenta() { return {255, 0, 255, 255}; }
    static constexpr Color RayWhite() { return {245, 245, 245, 255}; }

    [[nodiscard]] constexpr ::Color color() const { return _color; }

    [[nodiscard]] constexpr std::uint8_t r() const { return _color.r; }
    [[nodiscard]] constexpr std::uint8_t g() const { return _color.g; }
    [[nodiscard]] constexpr std::uint8_t b() const { return _color.b; }
    [[nodiscard]] constexpr std::uint8_t a() const { return _color.a; }

  protected:
  private:
    ::Color _color;
};
}  // namespace zappy::gui::raylib
