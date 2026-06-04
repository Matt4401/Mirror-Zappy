/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Texture2D
*/

#pragma once
#include <raylib.h>

#include <string>
#include <utility>

namespace zappy::gui::raylib::rtextures {
class Texture2D {
  public:
    explicit Texture2D(const std::string& path) : _texture{LoadTexture(path.c_str())} {}
    explicit Texture2D(::Texture2D texture) : _texture{texture} {}

    ~Texture2D() { reset(); }

    Texture2D(const Texture2D& other) = delete;
    Texture2D& operator=(const Texture2D& other) = delete;

    Texture2D(Texture2D&& other) noexcept : _texture{std::exchange(other._texture, {})} {}

    Texture2D& operator=(Texture2D&& other) noexcept {
        if (this != &other) {
            reset();
            _texture = std::exchange(other._texture, {});
        }
        return *this;
    }

    [[nodiscard]] bool valid() const { return IsTextureValid(_texture); }
    [[nodiscard]] int width() const { return _texture.width; }
    [[nodiscard]] int height() const { return _texture.height; }
    [[nodiscard]] ::Texture2D texture() const { return _texture; }

    void draw(::Vector2 position, Color tint) const { DrawTextureV(_texture, position, tint); }
    void draw(::Vector2 position, float rotation, float scale, Color tint) const {
        DrawTextureEx(_texture, position, rotation, scale, tint);
    }
    void draw(Rectangle source, ::Vector2 position, Color tint) const {
        DrawTextureRec(_texture, source, position, tint);
    }

  protected:
  private:
    void reset() {
        if (valid()) {
            UnloadTexture(_texture);
        }
        _texture = {};
    }

    ::Texture2D _texture{};
};
}  // namespace zappy::gui::raylib::rtextures
