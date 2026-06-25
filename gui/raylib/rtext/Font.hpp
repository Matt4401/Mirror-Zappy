/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Font
*/

#pragma once
#include <raylib.h>

#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Color.hpp"
#include "rmath/Vector2.hpp"

namespace zappy::gui::raylib::rtext {
class Font {
  public:
    Font() : _font{GetFontDefault()} {}
    explicit Font(const std::string& path) : _font{LoadFont(path.c_str())}, _owned{true} {}
    explicit Font(::Font font, bool owned = true) : _font{font}, _owned{owned} {}

    ~Font() { reset(); }

    Font(const Font& other) = delete;
    Font& operator=(const Font& other) = delete;

    Font(Font&& other) noexcept : _font{std::exchange(other._font, {})}, _owned{std::exchange(other._owned, false)} {}

    Font& operator=(Font&& other) noexcept {
        if (this != &other) {
            reset();
            _font = std::exchange(other._font, {});
            _owned = std::exchange(other._owned, false);
        }
        return *this;
    }

    [[nodiscard]] static Font fromMemory(std::string_view fileType, std::span<const unsigned char> data, int fontSize,
                                         std::span<const int> codepoints = {});

    [[nodiscard]] bool valid() const { return IsFontValid(_font); }
    [[nodiscard]] int baseSize() const { return _font.baseSize; }
    [[nodiscard]] int glyphCount() const { return _font.glyphCount; }
    [[nodiscard]] ::Font font() const { return _font; }

    [[nodiscard]] rmath::Vector2 measureTextEx(const std::string& text, float fontSize, float spacing) const;
    void drawTextEx(const std::string& text, rmath::Vector2 position, float fontSize, float spacing, Color color) const;

  protected:
  private:
    void reset();

    ::Font _font{};
    bool _owned = false;
};
}  // namespace zappy::gui::raylib::rtext
