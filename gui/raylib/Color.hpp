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

    static constexpr Color LightGray() { return LIGHTGRAY; }
    static constexpr Color Gray() { return GRAY; }
    static constexpr Color DarkGray() { return DARKGRAY; }
    static constexpr Color Yellow() { return YELLOW; }
    static constexpr Color Gold() { return GOLD; }
    static constexpr Color Orange() { return ORANGE; }
    static constexpr Color Pink() { return PINK; }
    static constexpr Color Red() { return RED; }
    static constexpr Color Maroon() { return MAROON; }
    static constexpr Color Green() { return GREEN; }
    static constexpr Color Lime() { return LIME; }
    static constexpr Color DarkGreen() { return DARKGREEN; }
    static constexpr Color SkyBlue() { return SKYBLUE; }
    static constexpr Color Blue() { return BLUE; }
    static constexpr Color DarkBlue() { return DARKBLUE; }
    static constexpr Color Purple() { return PURPLE; }
    static constexpr Color Violet() { return VIOLET; }
    static constexpr Color DarkPurple() { return DARKPURPLE; }
    static constexpr Color Beige() { return BEIGE; }
    static constexpr Color Brown() { return BROWN; }
    static constexpr Color DarkBrown() { return DARKBROWN; }
    static constexpr Color White() { return WHITE; }
    static constexpr Color Black() { return BLACK; }
    static constexpr Color Blank() { return BLANK; }
    static constexpr Color Magenta() { return MAGENTA; }
    static constexpr Color RayWhite() { return RAYWHITE; }

    [[nodiscard]] constexpr const ::Color& color() const { return _color; }

    [[nodiscard]] constexpr std::uint8_t r() const { return _color.r; }
    [[nodiscard]] constexpr std::uint8_t g() const { return _color.g; }
    [[nodiscard]] constexpr std::uint8_t b() const { return _color.b; }
    [[nodiscard]] constexpr std::uint8_t a() const { return _color.a; }

  protected:
  private:
    ::Color _color;
};
}  // namespace zappy::gui::raylib
