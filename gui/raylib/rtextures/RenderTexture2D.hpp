/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** RenderTexture2D
*/

#pragma once
#include <raylib.h>

#include <utility>

#include "Color.hpp"
#include "rmath/Vector2.hpp"

namespace zappy::gui::raylib::rtextures {
class RenderTexture2D {
  public:
    RenderTexture2D(int width, int height) : _renderTexture{LoadRenderTexture(width, height)} {}

    ~RenderTexture2D() { reset(); }

    RenderTexture2D(const RenderTexture2D& other) = delete;
    RenderTexture2D& operator=(const RenderTexture2D& other) = delete;

    RenderTexture2D(RenderTexture2D&& other) noexcept : _renderTexture{std::exchange(other._renderTexture, {})} {}

    RenderTexture2D& operator=(RenderTexture2D&& other) noexcept {
        if (this != &other) {
            reset();
            _renderTexture = std::exchange(other._renderTexture, {});
        }
        return *this;
    }

    void begin() const { BeginTextureMode(_renderTexture); }
    static void end() { EndTextureMode(); }

    [[nodiscard]] ::RenderTexture2D renderTexture() const { return _renderTexture; }
    [[nodiscard]] ::Texture2D rawTexture() const { return _renderTexture.texture; }

    void draw(rmath::Vector2 position, Color tint) const {
        Rectangle const source{.x = 0.0F,
                               .y = 0.0F,
                               .width = static_cast<float>(_renderTexture.texture.width),
                               .height = static_cast<float>(-_renderTexture.texture.height)};
        ::DrawTextureRec(_renderTexture.texture, source, position.vector(), tint.color());
    }

  private:
    void reset() {
        if (_renderTexture.id != 0) {
            UnloadRenderTexture(_renderTexture);
            _renderTexture = {};
        }
    }

    ::RenderTexture2D _renderTexture{};
};
}  // namespace zappy::gui::raylib::rtextures
