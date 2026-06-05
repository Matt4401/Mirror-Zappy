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

#include "rmath/Vector2.hpp"
#include "rtext/Font.hpp"

namespace zappy::gui::raylib::rtext {
class Text {
  public:
    Text() = default;
    explicit Text(std::string text) : _text{std::move(text)} {}

    [[nodiscard]] const std::string& value() const { return _text; }
    void setValue(std::string text) { _text = std::move(text); }

    void draw(int x, int y, int fontSize, Color color) const { DrawText(_text.c_str(), x, y, fontSize, color); }

    void draw(const Font& font, rmath::Vector2 position, float fontSize, float spacing, Color tint) const {
        DrawTextEx(font.font(), _text.c_str(), position.vector(), fontSize, spacing, tint);
    }

    void draw(const Font& font, rmath::Vector2 position, rmath::Vector2 origin, float rotation, float fontSize,
                 float spacing, Color tint) const {
        DrawTextPro(font.font(), _text.c_str(), position.vector(), origin.vector(), rotation, fontSize, spacing, tint);
    }

    [[nodiscard]] int measure(int fontSize) const { return MeasureText(_text.c_str(), fontSize); }

    [[nodiscard]] rmath::Vector2 measure(const Font& font, float fontSize, float spacing) const {
        return MeasureTextEx(font.font(), _text.c_str(), fontSize, spacing);
    }

    [[nodiscard]] int codepointCount() const { return GetCodepointCount(_text.c_str()); }

    [[nodiscard]] static std::string upper(const std::string& text) { return TextToUpper(text.c_str()); }
    [[nodiscard]] static std::string lower(const std::string& text) { return TextToLower(text.c_str()); }
    [[nodiscard]] static std::string pascal(const std::string& text) { return TextToPascal(text.c_str()); }
    [[nodiscard]] static std::string snake(const std::string& text) { return TextToSnake(text.c_str()); }
    [[nodiscard]] static std::string camel(const std::string& text) { return TextToCamel(text.c_str()); }

  protected:
  private:
    std::string _text;
};
}  // namespace zappy::gui::raylib::rtext
