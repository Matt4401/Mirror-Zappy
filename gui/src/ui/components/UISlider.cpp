/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UISlider
*/

#include "UISlider.hpp"

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <utility>

#include "Color.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Rectangle.hpp"
#include "rmath/Vector2.hpp"
#include "rshapes/Shapes.hpp"

namespace zappy::gui::ui::components {

using Shapes = raylib::rshapes::Shapes;

UISlider::UISlider(float x, float y, float width, float height, float min, float max, float initialValue)
    : _position(x, y), _size(width, height), _min(min), _max(max), _value(std::clamp(initialValue, min, max)) {
    if (min > max) {
        throw std::invalid_argument("UISlider: min must be <= max");
    }
    updateHandlePositionFromValue();
}

UISlider::UISlider(float x, float y, float width, float height, float min, float max, float initialValue,
                   raylib::Color trackColor, raylib::Color handleNormalColor)
    : _position(x, y),
      _size(width, height),
      _min(min),
      _max(max),
      _value(std::clamp(initialValue, min, max)),
      _trackColor(trackColor),
      _handleNormalColor(handleNormalColor) {
    if (min > max) {
        throw std::invalid_argument("UISlider: min must be <= max");
    }
    updateHandlePositionFromValue();
}

void UISlider::draw() {
    if (!_isVisible) {
        return;
    }

    Shapes::drawRectangleRec({.x = _position.x(), .y = _position.y(), .width = _size.x(), .height = _size.y()},
                             _trackColor);

    raylib::Color handleColor = _handleNormalColor;
    if (_isDragging) {
        handleColor = _handleDragColor;
    } else if (_isHovered) {
        handleColor = _handleHoverColor;
    }

    float const handleWidth = std::max(MinHandleWidth, _size.x() * HandleWidthRatio);
    Shapes::drawRectangleRec({.x = _handleX, .y = _position.y(), .width = handleWidth, .height = _size.y()},
                             handleColor);
}

void UISlider::update() {
    if (!_isVisible) {
        return;
    }

    float const handleWidth = std::max(MinHandleWidth, _size.x() * HandleWidthRatio);
    raylib::rmath::Vector2 const mousePos = raylib::rcore::Event::getMousePositionStatic();

    if (raylib::rcore::Window::isCursorHidden()) {
        _isHovered = false;
        if (_isDragging) {
            _isDragging = false;
        }
    } else {
        raylib::rmath::Rectangle const handleRec{
            .x = _handleX, .y = _position.y(), .width = handleWidth, .height = _size.y()};
        _isHovered = (mousePos.x() >= handleRec.x && mousePos.x() <= handleRec.x + handleRec.width &&
                      mousePos.y() >= handleRec.y && mousePos.y() <= handleRec.y + handleRec.height);
    }

    if (_isDragging) {
        _handleX = mousePos.x() - (handleWidth / 2.0F);

        float const minX = _position.x();
        float const maxX = _position.x() + _size.x() - handleWidth;
        _handleX = std::clamp(_handleX, minX, maxX);

        float const oldValue = _value;
        updateValueFromHandlePosition();
        if (oldValue != _value && _onValueChanged) {
            _onValueChanged(_value);
        }
    }
}

void UISlider::handleEvent() {
    if (!_isVisible) {
        return;
    }

    if (_isHovered && raylib::rcore::Event::isMouseButtonPressed(MouseLeftButton)) {
        _isDragging = true;
    }

    if (_isDragging && raylib::rcore::Event::isMouseButtonReleased(MouseLeftButton)) {
        _isDragging = false;
        if (_onValueChanged) {
            _onValueChanged(_value);
        }
    }
}

void UISlider::setPosition(float x, float y) {
    _position.setX(x);
    _position.setY(y);
    updateHandlePositionFromValue();
}

void UISlider::setSize(float width, float height) {
    _size = raylib::rmath::Vector2(width, height);
    updateHandlePositionFromValue();
}

bool UISlider::isVisible() const { return _isVisible; }

void UISlider::setVisible(bool visible) { _isVisible = visible; }

void UISlider::setOnValueChanged(std::function<void(float)> callback) { _onValueChanged = std::move(callback); }

void UISlider::setValue(float value) {
    if (_isDragging) {
        return;
    }
    _value = std::clamp(value, _min, _max);
    updateHandlePositionFromValue();
}

float UISlider::getValue() const { return _value; }

void UISlider::updateHandlePositionFromValue() {
    float const handleWidth = std::max(MinHandleWidth, _size.x() * HandleWidthRatio);
    float const range = _max - _min;
    float const trackWidth = _size.x() - handleWidth;

    if (range <= 0.0F) {
        _handleX = _position.x();
        return;
    }

    float const ratio = (_value - _min) / range;
    _handleX = _position.x() + (ratio * trackWidth);
}

void UISlider::updateValueFromHandlePosition() {
    float const handleWidth = std::max(MinHandleWidth, _size.x() * HandleWidthRatio);
    float const trackWidth = _size.x() - handleWidth;

    if (trackWidth <= 0.0F) {
        _value = _min;
        return;
    }

    float const ratio = (_handleX - _position.x()) / trackWidth;
    _value = _min + (ratio * (_max - _min));
}

}  // namespace zappy::gui::ui::components
