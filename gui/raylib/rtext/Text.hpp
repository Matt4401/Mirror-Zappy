/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Text
*/

#pragma once
#include <raylib.h>

#include <string>
#include <utility>

#include "Color.hpp"
#include "rmath/Vector2.hpp"
#include "rtext/Font.hpp"

namespace zappy::gui::raylib::rtext {
class Text {
  public:
    Text() = default;
    explicit Text(std::string text, rmath::Vector2 position) : _text{std::move(text)}, _position{position} {}

    [[nodiscard]] const std::string& value() const { return _text; }
    void setValue(std::string text) { _text = std::move(text); }

    void draw(int fontSize, zappy::gui::raylib::Color color) const {
        DrawText(_text.c_str(), static_cast<int>(_position.x()), static_cast<int>(_position.y()), fontSize,
                 color.color());
    }

    void draw(const Font& font, float fontSize, float spacing, zappy::gui::raylib::Color tint) const {
        DrawTextEx(font.font(), _text.c_str(), _position.vector(), fontSize, spacing, tint.color());
    }

    void draw(const Font& font, rmath::Vector2 origin, float rotation, float fontSize, float spacing,
              zappy::gui::raylib::Color tint) const {
        DrawTextPro(font.font(), _text.c_str(), _position.vector(), origin.vector(), rotation, fontSize, spacing,
                    tint.color());
    }

    [[nodiscard]] int measure(int fontSize) const { return MeasureText(_text.c_str(), fontSize); }

    [[nodiscard]] rmath::Vector2 measure(const Font& font, float fontSize, float spacing) const {
        return MeasureTextEx(font.font(), _text.c_str(), fontSize, spacing);
    }

    [[nodiscard]] static rmath::Vector2 measureText(const Font& font, const std::string& text, float fontSize,
                                                    float spacing) {
        return MeasureTextEx(font.font(), text.c_str(), fontSize, spacing);
    }

    [[nodiscard]] int codepointCount() const { return GetCodepointCount(_text.c_str()); }

    [[nodiscard]] std::string upper() { return TextToUpper(_text.c_str()); }
    [[nodiscard]] std::string lower() { return TextToLower(_text.c_str()); }
    [[nodiscard]] std::string pascal() { return TextToPascal(_text.c_str()); }
    [[nodiscard]] std::string snake() { return TextToSnake(_text.c_str()); }
    [[nodiscard]] std::string camel() { return TextToCamel(_text.c_str()); }

  protected:
  private:
    std::string _text;
    rmath::Vector2 _position{0, 0};
};
}  // namespace zappy::gui::raylib::rtext
