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

#include "Color.hpp"
#include "rmath/Vector2.hpp"

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
    [[nodiscard]] unsigned int id() const { return _texture.id; }

    void draw(rmath::Vector2 position, Color tint) const { DrawTextureV(_texture, position.vector(), tint.color()); }
    void draw(rmath::Vector2 position, float rotation, float scale, Color tint) const {
        DrawTextureEx(_texture, position.vector(), rotation, scale, tint.color());
    }
    void draw(Rectangle source, rmath::Vector2 position, Color tint) const {
        DrawTextureRec(_texture, source, position.vector(), tint.color());
    }
    void drawCover(int width, int height, zappy::gui::raylib::Color tint) const {
        drawCoverPanned(width, height, 0.5F, tint);
    }
    void drawCoverPanned(int width, int height, float horizontalPan, zappy::gui::raylib::Color tint) const;

  protected:
  private:
    void reset();
    ::Texture2D _texture{};
};
}  // namespace zappy::gui::raylib::rtextures
