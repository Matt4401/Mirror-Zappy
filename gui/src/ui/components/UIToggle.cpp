/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIToggle
*/

#include "UIToggle.hpp"

#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "Color.hpp"
#include "rcore/Event.hpp"
#include "rmath/Rectangle.hpp"
#include "rmath/Vector2.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"
#include "rtext/Text.hpp"

namespace zappy::gui::ui::components {

const raylib::Color UIToggle::BackgroundColor{245, 245, 245, 255};
const raylib::Color UIToggle::HoverColor{220, 220, 220, 255};
const raylib::Color UIToggle::BorderColor{150, 150, 150, 255};
const raylib::Color UIToggle::TextColor{30, 30, 30, 255};
const raylib::Color UIToggle::CheckColor{0, 200, 0, 255};

UIToggle::UIToggle(const float x, const float y, const float width, const float height, std::string text,
                   std::shared_ptr<raylib::rtext::Font> font, const bool initialState)
    : _position{x, y}, _size{width, height}, _state{initialState}, _text{std::move(text)}, _fontRef{std::move(font)} {
    updateTextPosition();
}

void UIToggle::draw() {
    if (!_isVisible) {
        return;
    }

    const raylib::rmath::Vector2 boxPos = {_position.x(), _position.y() + ((_size.y() - ToggleBoxSize) / 2.0F)};
    const raylib::rmath::Vector2 boxSize = {ToggleBoxSize, ToggleBoxSize};
    raylib::rmath::Rectangle const rec{.x = boxPos.x(), .y = boxPos.y(), .width = boxSize.x(), .height = boxSize.y()};

    raylib::rshapes::Shapes::drawRectangleRec(rec, _isHovered ? HoverColor : BackgroundColor);
    raylib::rshapes::Shapes::drawRectangleLinesEx(rec, 1.0F, BorderColor);

    if (_state) {
        const float padding = 4.0F;
        raylib::rmath::Rectangle const checkRec{.x = boxPos.x() + padding,
                                                .y = boxPos.y() + padding,
                                                .width = ToggleBoxSize - (padding * 2.0F),
                                                .height = ToggleBoxSize - (padding * 2.0F)};
        raylib::rshapes::Shapes::drawRectangleRec(checkRec, CheckColor);
    }

    if (_fontRef) {
        raylib::rtext::Text const textObj(_text, _textPosition);
        textObj.draw(*_fontRef, DefaultFontSize, 1.0F, TextColor);
    }
}

void UIToggle::update() {
    if (!_isVisible) {
        _isHovered = false;
        return;
    }

    const auto mousePos = raylib::rcore::Event::getMousePositionStatic();
    _isHovered = (mousePos.x() >= _position.x() && mousePos.x() <= _position.x() + _size.x() &&
                  mousePos.y() >= _position.y() && mousePos.y() <= _position.y() + _size.y());
}

void UIToggle::handleEvent() {
    if (!_isVisible) {
        return;
    }

    if (_isHovered && raylib::rcore::Event::isMouseButtonReleased(MouseLeftButton)) {
        setState(!_state);
    }
}

void UIToggle::setPosition(const float x, const float y) {
    _position = {x, y};
    updateTextPosition();
}

void UIToggle::setSize(const float width, const float height) {
    _size = {width, height};
    updateTextPosition();
}

bool UIToggle::isVisible() const { return _isVisible; }

void UIToggle::setVisible(const bool visible) { _isVisible = visible; }

bool UIToggle::isHovered() const { return _isHovered; }

void UIToggle::setOnToggle(std::function<void(bool)> callback) { _onToggle = std::move(callback); }

void UIToggle::setState(const bool state) {
    if (_state != state) {
        _state = state;
        if (_onToggle) {
            _onToggle(_state);
        }
    }
}

bool UIToggle::getState() const { return _state; }

void UIToggle::updateTextPosition() {
    if (!_fontRef) {
        return;
    }
    const raylib::rmath::Vector2 textSize = raylib::rtext::Text::measureText(*_fontRef, _text, DefaultFontSize, 1.0F);
    _textPosition = {_position.x() + ToggleBoxSize + TextPaddingX, _position.y() + ((_size.y() - textSize.y()) / 2.0F)};
}

}  // namespace zappy::gui::ui::components
