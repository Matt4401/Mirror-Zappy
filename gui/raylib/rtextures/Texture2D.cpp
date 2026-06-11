/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Texture2D
*/

#include "Texture2D.hpp"

#include <raylib.h>

#include "Color.hpp"

namespace zappy::gui::raylib::rtextures {
void Texture2D::drawCoverPanned(int width, int height, float horizontalPan, zappy::gui::raylib::Color tint) const {
    if (_texture.height == 0 || height == 0) {
        return;
    }
    const float textureRatio = static_cast<float>(_texture.width) / static_cast<float>(_texture.height);
    const float screenRatio = static_cast<float>(width) / static_cast<float>(height);
    Rectangle source(0.0F, 0.0F, static_cast<float>(_texture.width), static_cast<float>(_texture.height));

    if (textureRatio > screenRatio) {
        source.width = static_cast<float>(_texture.height) * screenRatio;
        source.x = (static_cast<float>(_texture.width) - source.width) * horizontalPan;
    } else {
        source.height = static_cast<float>(_texture.width) / screenRatio;
        source.y = (static_cast<float>(_texture.height) - source.height) * 0.5F;
    }

    DrawTexturePro(_texture, source, Rectangle(0.0F, 0.0F, static_cast<float>(width), static_cast<float>(height)),
                   Vector2(0.0F, 0.0F), 0.0F, tint.color());
}

void Texture2D::reset() {
    if (valid()) {
        UnloadTexture(_texture);
    }
    _texture = {};
}
}  // namespace zappy::gui::raylib::rtextures
