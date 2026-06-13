/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIButton
*/

#include "UIButton.hpp"

#include <raylib.h>

#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "Color.hpp"
#include "rcore/Event.hpp"
#include "rmath/Vector2.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"
#include "rtext/Text.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::components {

const raylib::Color UIButton::OuterBorderColor(0, 0, 0, 255);
const raylib::Color UIButton::NormalFillColor(120, 120, 120, 255);
const raylib::Color UIButton::HoveredFillColor(160, 160, 190, 255);
const raylib::Color UIButton::PressedFillColor(80, 80, 80, 255);
const raylib::Color UIButton::BrightBorderColor(180, 180, 180, 255);
const raylib::Color UIButton::DarkBorderColor(60, 60, 60, 255);
const raylib::Color UIButton::HoverOutlineColor(255, 255, 255, 180);
const raylib::Color UIButton::TextHoverColor(255, 255, 255, 255);
const raylib::Color UIButton::TextNormalColor(220, 220, 220, 255);

UIButton::UIButton(float x, float y, float width, float height, const std::string& text,
                   std::shared_ptr<raylib::rtext::Font> font)
    : _position(x, y), _size(width, height), _fontRef(std::move(font)) {
    _label = std::make_unique<UIText>(text, _fontRef);
    updateTextPosition();
}

void UIButton::updateTextPosition() {
    if (!_label) {
        return;
    }
    int const fontSize = _fontRef && _fontRef->valid() ? _fontRef->baseSize() : static_cast<int>(DefaultFontSize);
    raylib::rtext::Font const defaultFont;
    float const textWidth = raylib::rtext::Text::measureText(_fontRef && _fontRef->valid() ? *_fontRef : defaultFont,
                                                             _label->text(), static_cast<float>(fontSize), TextSpacing)
                                .x();
    auto textHeight = static_cast<float>(fontSize);

    float const offsetX = _isPressed ? PressedOffset : 0.0F;
    float const offsetY = _isPressed ? PressedOffset : 0.0F;

    _label->setPosition(_position.x() + ((_size.x() - textWidth) / 2.0F) + offsetX,
                        _position.y() + ((_size.y() - textHeight) / 2.0F) + offsetY);
}

void UIButton::draw() {
    if (!_isVisible) {
        return;
    }

    Rectangle const rec = {_position.x(), _position.y(), _size.x(), _size.y()};

    raylib::Color innerFill = _isHovered ? HoveredFillColor : NormalFillColor;
    raylib::Color topBorder = BrightBorderColor;
    raylib::Color bottomBorder = DarkBorderColor;

    if (_isPressed) {
        innerFill = PressedFillColor;
        topBorder = DarkBorderColor;
        bottomBorder = BrightBorderColor;
    }

    raylib::rshapes::Shapes::drawRectangleRec(rec, OuterBorderColor);

    raylib::rshapes::Shapes::drawRectangleRec({rec.x + BorderThickness, rec.y + BorderThickness,
                                               rec.width - (BorderThickness * 2), rec.height - (BorderThickness * 2)},
                                              innerFill);

    raylib::rshapes::Shapes::drawRectangleRec(
        {rec.x + BorderThickness, rec.y + BorderThickness, rec.width - (BorderThickness * 2), BorderThickness},
        topBorder);
    raylib::rshapes::Shapes::drawRectangleRec(
        {rec.x + BorderThickness, rec.y + BorderThickness, BorderThickness, rec.height - (BorderThickness * 2)},
        topBorder);

    raylib::rshapes::Shapes::drawRectangleRec({rec.x + BorderThickness, rec.y + rec.height - (BorderThickness * 2),
                                               rec.width - (BorderThickness * 2), BorderThickness},
                                              bottomBorder);
    raylib::rshapes::Shapes::drawRectangleRec({rec.x + rec.width - (BorderThickness * 2), rec.y + BorderThickness,
                                               BorderThickness, rec.height - (BorderThickness * 2)},
                                              bottomBorder);

    if (_isHovered && !_isPressed) {
        raylib::rshapes::Shapes::drawRectangleLinesEx(
            {rec.x + OutlineThickness, rec.y + OutlineThickness, rec.width - (OutlineThickness * 2),
             rec.height - (OutlineThickness * 2)},
            OutlineThickness, HoverOutlineColor);
    }

    if (_label) {
        _label->setColor(_isHovered ? TextHoverColor : TextNormalColor);
        _label->draw();
    }
}

void UIButton::update() {
    if (!_isVisible) {
        return;
    }

    raylib::rmath::Vector2 const mousePos = raylib::rcore::Event::getMousePositionStatic();
    Rectangle const rec = {_position.x(), _position.y(), _size.x(), _size.y()};
    _isHovered = (mousePos.x() >= rec.x && mousePos.x() <= rec.x + rec.width && mousePos.y() >= rec.y &&
                  mousePos.y() <= rec.y + rec.height);

    _isPressed = _isHovered && raylib::rcore::Event::isMouseButtonDown(MouseLeftButton);

    updateTextPosition();

    if (_label) {
        _label->update();
    }
}

void UIButton::handleEvent(const raylib::rcore::Event& event) {
    if (!_isVisible) {
        return;
    }

    if (_isHovered && raylib::rcore::Event::isMouseButtonReleased(MouseLeftButton)) {
        if (_onClick) {
            _onClick();
        }
    }

    if (_label) {
        _label->handleEvent(event);
    }
}

void UIButton::setPosition(float x, float y) {
    _position.setX(x);
    _position.setY(y);
    updateTextPosition();
}

void UIButton::setSize(float width, float height) {
    _size = raylib::rmath::Vector2(width, height);
    updateTextPosition();
}

bool UIButton::isVisible() const { return _isVisible; }

void UIButton::setVisible(bool visible) { _isVisible = visible; }

void UIButton::setOnClick(std::function<void()> callback) { _onClick = std::move(callback); }

}  // namespace zappy::gui::ui::components
