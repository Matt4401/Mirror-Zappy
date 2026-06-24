/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UICompass
*/

#include "UICompass.hpp"

#include <raylib.h>

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>

#include "Color.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Window.hpp"
#include "rmath/Rectangle.hpp"
#include "rmath/Vector3.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"

namespace zappy::gui::ui::components {

namespace {
constexpr float CompassFovDeg = 120.0F;
constexpr int MajorTickSpacingDeg = 15;
constexpr float TickHeightMajor = 15.0F;
constexpr float TickHeightMinor = 8.0F;
constexpr float MarkerSize = 10.0F;
constexpr int FontSize = 16;
}  // namespace

UICompass::UICompass(float x, float y, float width, float height, raylib::rcore::Camera& camera,
                     const std::shared_ptr<raylib::rtext::Font>& font)
    : _x(x), _y(y), _width(width), _height(height), _camera(camera), _font(font) {}

void UICompass::draw() {
    if (!_isVisible) {
        return;
    }

    raylib::rmath::Rectangle const rec{.x = _x, .y = _y, .width = _width, .height = _height};
    raylib::rshapes::Shapes::drawRectangleRec(rec, raylib::Color(30, 30, 30, 200));
    raylib::rshapes::Shapes::drawRectangleLinesEx(rec, 1.0F, raylib::Color::Gray());

    auto target = _camera.get().target();
    auto pos = _camera.get().position();
    float const dx = target.x() - pos.x();
    float const dz = target.z() - pos.z();

    float const angleRad = std::atan2(dx, dz);
    float angleDeg = angleRad * (180.0F / PI);
    if (angleDeg < 0.0F) {
        angleDeg += 360.0F;
    }

    raylib::rcore::Window::beginScissorMode(static_cast<int>(_x), static_cast<int>(_y), static_cast<int>(_width),
                                            static_cast<int>(_height));

    float const centerX = _x + (_width / 2.0F);

    float const pixelsPerDegree = _width / CompassFovDeg;

    int const startDeg = static_cast<int>(angleDeg - (CompassFovDeg / 2.0F)) - MajorTickSpacingDeg;
    int const endDeg = static_cast<int>(angleDeg + (CompassFovDeg / 2.0F)) + MajorTickSpacingDeg;

    for (int deg = startDeg; deg <= endDeg; ++deg) {
        if (deg % MajorTickSpacingDeg != 0) {
            continue;
        }

        int normalizedDeg = deg % 360;
        if (normalizedDeg < 0) {
            normalizedDeg += 360;
        }

        float const offsetDeg = static_cast<float>(deg) - angleDeg;
        float const tickX = centerX + (offsetDeg * pixelsPerDegree);

        bool const isCardinal = (normalizedDeg % 90 == 0);
        float const tickH = isCardinal ? TickHeightMajor : TickHeightMinor;

        float const tickY = _y + _height - tickH - 2.0F;
        raylib::rshapes::Shapes::drawLineEx({tickX, tickY}, {tickX, tickY + tickH}, 2.0F, raylib::Color::White());

        if (isCardinal) {
            std::string label;
            if (normalizedDeg == 0) {
                label = "S";
            } else if (normalizedDeg == 90) {
                label = "E";
            } else if (normalizedDeg == 180) {
                label = "N";
            } else if (normalizedDeg == 270) {
                label = "W";
            }

            if (!label.empty() && _font && _font->valid()) {
                float const textW = _font->measureTextEx(label, FontSize, 1.0F).x();
                float const textX = tickX - (textW / 2.0F);
                float const textY = _y + 5.0F;
                _font->drawTextEx(label, {textX, textY}, FontSize, 1.0F, raylib::Color::White());
            }
        }
    }

    raylib::rcore::Window::endScissorMode();

    raylib::rshapes::Shapes::drawTriangle({centerX - (MarkerSize / 2.0F), _y + _height},
                                          {centerX, _y + _height - MarkerSize},
                                          {centerX + (MarkerSize / 2.0F), _y + _height}, raylib::Color::Red());
}

void UICompass::update() {
    if (!_isVisible) {
        return;
    }

    auto const screenW = static_cast<float>(raylib::rcore::Window::screenWidth());

    _width = std::clamp(screenW * 0.3F, 300.0F, 800.0F);
    _height = 40.0F;
    _x = (screenW - _width) / 2.0F;
    _y = 15.0F;
}

void UICompass::handleEvent() {}

void UICompass::setPosition(float x, float y) {
    _x = x;
    _y = y;
}

void UICompass::setSize(float width, float height) {
    _width = width;
    _height = height;
}

bool UICompass::isVisible() const { return _isVisible; }

bool UICompass::isHovered() const { return false; }

void UICompass::setVisible(bool visible) { _isVisible = visible; }

}  // namespace zappy::gui::ui::components
