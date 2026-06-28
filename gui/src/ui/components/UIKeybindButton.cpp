/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIKeybindButton
*/

#include "UIKeybindButton.hpp"

#include <raylib.h>

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

const raylib::Color UIKeybindButton::BackgroundColor{245, 245, 245, 255};
const raylib::Color UIKeybindButton::HoverColor{220, 220, 220, 255};
const raylib::Color UIKeybindButton::WaitingColor{255, 200, 200, 255};
const raylib::Color UIKeybindButton::BorderColor{150, 150, 150, 255};
const raylib::Color UIKeybindButton::TextColor{30, 30, 30, 255};

UIKeybindButton::UIKeybindButton(const float x, const float y, const float width, const float height, std::string label,
                                 const int initialKey, std::shared_ptr<raylib::rtext::Font> font)
    : _position{x, y},
      _size{width, height},
      _label{std::move(label)},
      _currentKey{initialKey},
      _fontRef{std::move(font)} {
    updateTextPositions();
}

void UIKeybindButton::draw() {
    if (!_isVisible) {
        return;
    }

    if (_fontRef) {
        raylib::rtext::Text const labelObj(_label, _labelPosition);
        labelObj.draw(*_fontRef, DefaultFontSize, 1.0F, TextColor);
    }

    raylib::Color bgColor = BackgroundColor;
    if (_isWaitingForInput) {
        bgColor = WaitingColor;
    } else if (_isHovered) {
        bgColor = HoverColor;
    }
    raylib::rmath::Rectangle const rec{
        .x = _buttonPosition.x(), .y = _buttonPosition.y(), .width = _buttonSize.x(), .height = _buttonSize.y()};
    raylib::rshapes::Shapes::drawRectangleRec(rec, bgColor);
    raylib::rshapes::Shapes::drawRectangleLinesEx(rec, 1.0F, BorderColor);

    if (_fontRef) {
        std::string const valueText = _isWaitingForInput ? "..." : keyToString(_currentKey);
        const raylib::rmath::Vector2 textSize =
            raylib::rtext::Text::measureText(*_fontRef, valueText, DefaultFontSize, 1.0F);
        const raylib::rmath::Vector2 textPos = {_buttonPosition.x() + ((_buttonSize.x() - textSize.x()) / 2.0F),
                                                _buttonPosition.y() + ((_buttonSize.y() - textSize.y()) / 2.0F)};
        raylib::rtext::Text const valueObj(valueText, textPos);
        valueObj.draw(*_fontRef, DefaultFontSize, 1.0F, TextColor);
    }
}

void UIKeybindButton::update() {
    if (!_isVisible) {
        _isHovered = false;
        return;
    }

    const auto mousePos = raylib::rcore::Event::getMousePositionStatic();
    _isHovered = (mousePos.x() >= _buttonPosition.x() && mousePos.x() <= _buttonPosition.x() + _buttonSize.x() &&
                  mousePos.y() >= _buttonPosition.y() && mousePos.y() <= _buttonPosition.y() + _buttonSize.y());
}

void UIKeybindButton::handleWaitingInput() {
    int const key = GetKeyPressed();
    if (key != 0) {
        _currentKey = key;
        _isWaitingForInput = false;
        if (_onKeyBind) {
            _onKeyBind(_currentKey);
        }
        return;
    }

    bool mouseBound = false;
    for (int i = 0; i <= MOUSE_BUTTON_BACK; i++) {
        if (IsMouseButtonPressed(i)) {
            if (i == MOUSE_BUTTON_LEFT && _isHovered) {
                _isWaitingForInput = false;
                mouseBound = true;
                break;
            }
            _currentKey = MouseButtonOffset + i;
            _isWaitingForInput = false;
            if (_onKeyBind) {
                _onKeyBind(_currentKey);
            }
            mouseBound = true;
            break;
        }
    }
    if (!mouseBound && raylib::rcore::Event::isMouseButtonReleased(MouseLeftButton) && !_isHovered) {
        _isWaitingForInput = false;
    }
}

void UIKeybindButton::handleEvent() {
    if (!_isVisible) {
        return;
    }

    if (_isWaitingForInput) {
        handleWaitingInput();
    } else {
        if (_isHovered && raylib::rcore::Event::isMouseButtonReleased(MouseLeftButton)) {
            _isWaitingForInput = true;
        }
    }
}

void UIKeybindButton::setPosition(const float x, const float y) {
    _position = {x, y};
    updateTextPositions();
}

void UIKeybindButton::setSize(const float width, const float height) {
    _size = {width, height};
    updateTextPositions();
}

bool UIKeybindButton::isVisible() const { return _isVisible; }

void UIKeybindButton::setVisible(const bool visible) { _isVisible = visible; }

bool UIKeybindButton::isHovered() const { return _isHovered; }

void UIKeybindButton::setOnKeyBind(std::function<void(int)> callback) { _onKeyBind = std::move(callback); }

void UIKeybindButton::setKey(const int key) { _currentKey = key; }

int UIKeybindButton::getKey() const { return _currentKey; }

void UIKeybindButton::updateTextPositions() {
    _buttonSize = {_size.x() * ButtonWidthRatio, _size.y()};
    _buttonPosition = {_position.x() + _size.x() - _buttonSize.x(), _position.y()};

    if (_fontRef) {
        const raylib::rmath::Vector2 labelSize =
            raylib::rtext::Text::measureText(*_fontRef, _label, DefaultFontSize, 1.0F);
        _labelPosition = {_position.x(), _position.y() + ((_size.y() - labelSize.y()) / 2.0F)};
    }
}

[[nodiscard]] std::string UIKeybindButton::keyToString(const int key) {
    if (key >= MouseButtonOffset && key <= MaxMouseButtonOffset) {
        switch (key - MouseButtonOffset) {
            case MOUSE_BUTTON_LEFT:
                return "MOUSE LEFT";
            case MOUSE_BUTTON_RIGHT:
                return "MOUSE RIGHT";
            case MOUSE_BUTTON_MIDDLE:
                return "MOUSE MIDDLE";
            case MOUSE_BUTTON_SIDE:
                return "MOUSE SIDE";
            case MOUSE_BUTTON_EXTRA:
                return "MOUSE EXTRA";
            case MOUSE_BUTTON_FORWARD:
                return "MOUSE FORWARD";
            case MOUSE_BUTTON_BACK:
                return "MOUSE BACK";
            default:
                return "MOUSE " + std::to_string(key - MouseButtonOffset);
        }
    }

    if (key == KEY_Q) {
        return "A";
    }
    if (key == KEY_W) {
        return "Z";
    }
    if (key == KEY_A) {
        return "Q";
    }
    if (key == KEY_Z) {
        return "W";
    }
    if (key == KEY_M) {
        return ",";
    }
    if (key == KEY_COMMA) {
        return ";";
    }
    if (key == KEY_SEMICOLON) {
        return "M";
    }

    if (key > AsciiSpace && key <= AsciiTilde) {
        return {std::string(1, static_cast<char>(key))};
    }
    switch (key) {
        case KEY_SPACE:
            return "SPACE";
        case KEY_ESCAPE:
            return "ESC";
        case KEY_ENTER:
            return "ENTER";
        case KEY_TAB:
            return "TAB";
        case KEY_BACKSPACE:
            return "BACKSPACE";
        case KEY_RIGHT:
            return "RIGHT";
        case KEY_LEFT:
            return "LEFT";
        case KEY_DOWN:
            return "DOWN";
        case KEY_UP:
            return "UP";
        case KEY_LEFT_SHIFT:
            return "LSHIFT";
        case KEY_RIGHT_SHIFT:
            return "RSHIFT";
        case KEY_LEFT_CONTROL:
            return "LCTRL";
        case KEY_RIGHT_CONTROL:
            return "RCTRL";
        case KEY_LEFT_ALT:
            return "LALT";
        case KEY_RIGHT_ALT:
            return "RALT";
        default:
            return "KEY " + std::to_string(key);
    }
}

}  // namespace zappy::gui::ui::components
