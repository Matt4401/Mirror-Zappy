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
                                         std::span<const int> codepoints = {}) {
        const std::string extension{fileType};
        return Font{LoadFontFromMemory(extension.c_str(), data.data(), static_cast<int>(data.size()), fontSize,
                                       codepoints.data(), static_cast<int>(codepoints.size()))};
    }

    [[nodiscard]] bool valid() const { return IsFontValid(_font); }
    [[nodiscard]] int baseSize() const { return _font.baseSize; }
    [[nodiscard]] int glyphCount() const { return _font.glyphCount; }
    [[nodiscard]] ::Font font() const { return _font; }

  protected:
  private:
    void reset() {
        if (_owned && valid()) {
            UnloadFont(_font);
        }
        _font = {};
        _owned = false;
    }

    ::Font _font{};
    bool _owned = false;
};
}  // namespace zappy::gui::raylib::rtext
