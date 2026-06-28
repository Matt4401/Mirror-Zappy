/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIButton
*/

#include "UIButton.hpp"

#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "Color.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Rectangle.hpp"
#include "rmath/Vector2.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"
#include "rtext/Text.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::components {

using Shapes = raylib::rshapes::Shapes;

const raylib::Color UIButton::OuterBorderColor(0, 0, 0, 255);
const raylib::Color UIButton::NormalFillColor(120, 120, 120, 255);
const raylib::Color UIButton::HoveredFillColor(160, 160, 190, 255);
const raylib::Color UIButton::PressedFillColor(80, 80, 80, 255);
const raylib::Color UIButton::BrightBorderColor(180, 180, 180, 255);
const raylib::Color UIButton::DarkBorderColor(60, 60, 60, 255);
const raylib::Color UIButton::HoverOutlineColor(255, 255, 255, 180);
const raylib::Color UIButton::TextHoverColor(255, 255, 255, 255);
const raylib::Color UIButton::TextNormalColor(220, 220, 220, 255);
std::function<void()> UIButton::_clickSoundHandler;

UIButton::UIButton(float x, float y, float width, float height, const std::string& text,
                   std::shared_ptr<raylib::rtext::Font> font)
    : _position(x, y), _size(width, height), _fontRef(std::move(font)), _fontSize(DefaultFontSize) {
    _label = std::make_unique<UIText>(text, _fontRef);
    updateTextPosition();
}

void UIButton::updateTextPosition() {
    if (!_label) {
        return;
    }
    int const fontSize = static_cast<int>(_fontSize);
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

    raylib::rmath::Rectangle const rec{.x = _position.x(), .y = _position.y(), .width = _size.x(), .height = _size.y()};

    raylib::Color innerFill = _isHovered ? HoveredFillColor : NormalFillColor;
    raylib::Color topBorder = BrightBorderColor;
    raylib::Color bottomBorder = DarkBorderColor;

    if (_isPressed) {
        innerFill = PressedFillColor;
        topBorder = DarkBorderColor;
        bottomBorder = BrightBorderColor;
    }

    Shapes::drawRectangleRec(rec, OuterBorderColor);

    Shapes::drawRectangleRec({.x = rec.x + BorderThickness,
                              .y = rec.y + BorderThickness,
                              .width = rec.width - (BorderThickness * 2),
                              .height = rec.height - (BorderThickness * 2)},
                             innerFill);

    Shapes::drawRectangleRec({.x = rec.x + BorderThickness,
                              .y = rec.y + BorderThickness,
                              .width = rec.width - (BorderThickness * 2),
                              .height = BorderThickness},
                             topBorder);
    Shapes::drawRectangleRec({.x = rec.x + BorderThickness,
                              .y = rec.y + BorderThickness,
                              .width = BorderThickness,
                              .height = rec.height - (BorderThickness * 2)},
                             topBorder);

    Shapes::drawRectangleRec({.x = rec.x + BorderThickness,
                              .y = rec.y + rec.height - (BorderThickness * 2),
                              .width = rec.width - (BorderThickness * 2),
                              .height = BorderThickness},
                             bottomBorder);
    Shapes::drawRectangleRec({.x = rec.x + rec.width - (BorderThickness * 2),
                              .y = rec.y + BorderThickness,
                              .width = BorderThickness,
                              .height = rec.height - (BorderThickness * 2)},
                             bottomBorder);

    if (_isHovered && !_isPressed) {
        Shapes::drawRectangleLinesEx({.x = rec.x + OutlineThickness,
                                      .y = rec.y + OutlineThickness,
                                      .width = rec.width - (OutlineThickness * 2),
                                      .height = rec.height - (OutlineThickness * 2)},
                                     OutlineThickness, HoverOutlineColor);
    }

    if (_label) {
        _label->setFontSize(_fontSize);
        _label->setColor(_isHovered ? TextHoverColor : TextNormalColor);
        _label->draw();
    }
}

void UIButton::update() {
    if (!_isVisible) {
        return;
    }

    if (raylib::rcore::Window::isCursorHidden()) {
        _isHovered = false;
        _isPressed = false;
    } else {
        raylib::rmath::Vector2 const mousePos = raylib::rcore::Event::getMousePositionStatic();
        raylib::rmath::Rectangle const rec{
            .x = _position.x(), .y = _position.y(), .width = _size.x(), .height = _size.y()};
        _isHovered = (mousePos.x() >= rec.x && mousePos.x() <= rec.x + rec.width && mousePos.y() >= rec.y &&
                      mousePos.y() <= rec.y + rec.height);

        _isPressed = _isHovered && raylib::rcore::Event::isMouseButtonDown(MouseLeftButton);
    }

    updateTextPosition();

    if (_label) {
        _label->update();
    }
}

void UIButton::handleEvent() {
    if (!_isVisible) {
        return;
    }

    if (_isHovered && raylib::rcore::Event::isMouseButtonReleased(MouseLeftButton)) {
        if (_clickSoundHandler) {
            _clickSoundHandler();
        }
        if (_onClick) {
            _onClick();
        }
    }

    if (_label) {
        _label->handleEvent();
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

void UIButton::setClickSoundHandler(std::function<void()> callback) { _clickSoundHandler = std::move(callback); }

void UIButton::setFontSize(float size) {
    if (size <= 0.0F) {
        return;
    }
    _fontSize = size;
    updateTextPosition();
}

void UIButton::setText(const std::string& text) {
    if (_label) {
        _label->setText(text);
        updateTextPosition();
    }
}

}  // namespace zappy::gui::ui::components
