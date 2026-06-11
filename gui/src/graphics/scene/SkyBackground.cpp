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
    if (_dayProgress < MORNING_START) {
        return raylib::Color::lerp(NIGHT, SUNRISE, _dayProgress / MORNING_START);
    }
    if (_dayProgress < AFTERNOON_START) {
        return raylib::Color::lerp(SUNRISE, DAY, (_dayProgress - MORNING_START) / (AFTERNOON_START - MORNING_START));
    }
    if (_dayProgress < EVENING_START) {
        return DAY;
    }
    if (_dayProgress < NIGHT_START) {
        return raylib::Color::lerp(DAY, SUNSET, (_dayProgress - EVENING_START) / (NIGHT_START - EVENING_START));
    }
    return raylib::Color::lerp(SUNSET, NIGHT, (_dayProgress - NIGHT_START) / (FULL_DAY - NIGHT_START));
}

}  // namespace zappy::gui::graphics::scene
