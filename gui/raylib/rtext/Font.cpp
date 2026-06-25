/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Font
*/

#include "Font.hpp"

#include <raylib.h>

#include <span>
#include <string>
#include <string_view>

#include "Color.hpp"
#include "rmath/Vector2.hpp"

namespace zappy::gui::raylib::rtext {
void Font::reset() {
    if (_owned && valid()) {
        UnloadFont(_font);
    }
    _font = {};
    _owned = false;
}

Font Font::fromMemory(std::string_view fileType, std::span<const unsigned char> data, int fontSize,
                      std::span<const int> codepoints) {
    const std::string extension{fileType};
    return Font{LoadFontFromMemory(extension.c_str(), data.data(), static_cast<int>(data.size()), fontSize,
                                   codepoints.data(), static_cast<int>(codepoints.size()))};
}

rmath::Vector2 Font::measureTextEx(const std::string& text, float fontSize, float spacing) const {
    ::Vector2 const size = ::MeasureTextEx(_font, text.c_str(), fontSize, spacing);
    return {size.x, size.y};
}

void Font::drawTextEx(const std::string& text, rmath::Vector2 position, float fontSize, float spacing,
                      Color color) const {
    ::DrawTextEx(_font, text.c_str(), position.vector(), fontSize, spacing, color.color());
}

}  // namespace zappy::gui::raylib::rtext
