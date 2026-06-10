/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Font
*/

#include "Font.hpp"
#include <raylib.h>
#include <string>
#include <span>
#include <string_view>

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
}