/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** SkyBackground
*/

#include "SkyBackground.hpp"

#include <cmath>

#include "Color.hpp"
#include "rcore/Window.hpp"

namespace zappy::gui::graphics::scene {

void SkyBackground::update(float deltaTime) {
    _dayProgress = std::fmod(_dayProgress + (deltaTime * DAY_SPEED), FULL_DAY);
    if (_dayProgress < 0.0F) {
        _dayProgress += FULL_DAY;
    }
}

void SkyBackground::draw(raylib::rcore::Window& window) const {
    _texture.drawCoverPanned(window.getScreenWidth(), window.getScreenHeight(), 0.5F, currentTint());
}

raylib::Color SkyBackground::currentTint() const {
    if (_dayProgress < 0.20F) {
        return raylib::Color::lerp(NIGHT, SUNRISE, _dayProgress / 0.20F);
    }
    if (_dayProgress < 0.45F) {
        return raylib::Color::lerp(SUNRISE, DAY, (_dayProgress - 0.20F) / 0.25F);
    }
    if (_dayProgress < 0.72F) {
        return DAY;
    }
    if (_dayProgress < 0.88F) {
        return raylib::Color::lerp(DAY, SUNSET, (_dayProgress - 0.72F) / 0.16F);
    }
    return raylib::Color::lerp(SUNSET, NIGHT, (_dayProgress - 0.88F) / 0.12F);
}

}  // namespace zappy::gui::graphics::scene
