/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Texture2D
*/

#include "Texture2D.hpp"

#include <raylib.h>

#include "Color.hpp"
#include "Image.hpp"
#include "rmath/Rectangle.hpp"
#include "rmath/Vector2.hpp"

namespace zappy::gui::raylib::rtextures {
void Texture2D::drawCoverPanned(int width, int height, float horizontalPan, zappy::gui::raylib::Color tint) const {
    if (_texture.height == 0 || height == 0) {
        return;
    }
    const float textureRatio = static_cast<float>(_texture.width) / static_cast<float>(_texture.height);
    const float screenRatio = static_cast<float>(width) / static_cast<float>(height);
    rmath::Rectangle source{.x = 0.0F,
                            .y = 0.0F,
                            .width = static_cast<float>(_texture.width),
                            .height = static_cast<float>(_texture.height)};

    if (textureRatio > screenRatio) {
        source.width = static_cast<float>(_texture.height) * screenRatio;
        source.x = (static_cast<float>(_texture.width) - source.width) * horizontalPan;
    } else {
        source.height = static_cast<float>(_texture.width) / screenRatio;
        source.y = (static_cast<float>(_texture.height) - source.height) * 0.5F;
    }

    rmath::Rectangle const dest{
        .x = 0.0F, .y = 0.0F, .width = static_cast<float>(width), .height = static_cast<float>(height)};
    rmath::Vector2 const origin{0.0F, 0.0F};

    ::DrawTexturePro(_texture,
                     ::Rectangle{.x = source.x, .y = source.y, .width = source.width, .height = source.height},
                     ::Rectangle{.x = dest.x, .y = dest.y, .width = dest.width, .height = dest.height}, origin.vector(),
                     0.0F, tint.color());
}

void Texture2D::reset() {
    if (_ownsTexture && _texture.id != 0) {
        UnloadTexture(_texture);
    }
    _texture = {};
}

Texture2D Texture2D::loadCubemap(const Image& image, int layoutType) {
    return Texture2D{LoadTextureCubemap(image.image(), layoutType)};
}
}  // namespace zappy::gui::raylib::rtextures
