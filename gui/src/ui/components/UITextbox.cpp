/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UITextbox
*/

#include "UITextbox.hpp"

#include <raylib.h>

#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "Color.hpp"
#include "components/UIText.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Rectangle.hpp"
#include "rmath/Vector2.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"

namespace zappy::gui::ui::components {

using Shapes = raylib::rshapes::Shapes;

UITextbox::UITextbox(float x, float y, float width, float height, const std::shared_ptr<raylib::rtext::Font>& font,
                     std::string placeholder)
    : _position(x, y), _size(width, height), _placeholder(std::move(placeholder)), _fontRef(font) {
    _label = std::make_unique<UIText>("", _fontRef);
    _label->setFontSize(static_cast<int>(DefaultFontSize));
    updateTextPosition();
}

void UITextbox::draw() {
    if (!_isVisible) {
        return;
    }

    raylib::rmath::Rectangle const rec{.x = _position.x(), .y = _position.y(), .width = _size.x(), .height = _size.y()};

    Shapes::drawRectangleRec(rec, BackgroundColor);

    raylib::Color const borderColor = _isFocused ? BorderFocusedColor : BorderNormalColor;
    Shapes::drawRectangleLinesEx(rec, 2.0F, borderColor);

    std::string displayText = _text;
    if (_text.empty() && !_isFocused) {
        displayText = _placeholder;
        _label->setColor(PlaceholderColor);
    } else {
        _label->setColor(TextColor);
        if (_isFocused) {
            if (static_cast<int>(::GetTime() * 2.0) % 2 == 0) {
                displayText += "|";
            }
        }
    }

    _label->setText(displayText);
    _label->draw();
}

void UITextbox::update() {
    if (!_isVisible) {
        return;
    }

    updateHoverState();

    if (_isFocused) {
        handleInput();
    }
}

void UITextbox::updateHoverState() {
    raylib::rmath::Vector2 const mousePos = raylib::rcore::Event::getMousePositionStatic();
    raylib::rmath::Rectangle const rec{.x = _position.x(), .y = _position.y(), .width = _size.x(), .height = _size.y()};

    if (raylib::rcore::Window::isCursorHidden()) {
        _isHovered = false;
    } else {
        _isHovered = (mousePos.x() >= rec.x && mousePos.x() <= rec.x + rec.width && mousePos.y() >= rec.y &&
                      mousePos.y() <= rec.y + rec.height);
    }
}

void UITextbox::handleInput() {
    int key = raylib::rcore::Event::getCharPressed();
    while (key > 0) {
        if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z')) {
            _text += static_cast<char>(key);
            updateTextPosition();
        }
        key = raylib::rcore::Event::getCharPressed();
    }
    if (raylib::rcore::Event::isKeyPressed(KeyBackspace) || raylib::rcore::Event::isKeyPressed(KEY_BACKSPACE)) {
        if (!_text.empty()) {
            _text.pop_back();
            updateTextPosition();
        }
    }
    if (raylib::rcore::Event::isKeyPressed(KeyEnter) || raylib::rcore::Event::isKeyPressed(KeyKpEnter)) {
        if (_onSubmit) {
            _onSubmit(_text);
        }
        _isFocused = false;
    }
}

void UITextbox::handleEvent() {
    if (!_isVisible) {
        return;
    }

    if (raylib::rcore::Event::isMouseButtonPressed(MouseLeftButton)) {
        _isFocused = _isHovered;
    }
}

void UITextbox::setPosition(float x, float y) {
    _position.setX(x);
    _position.setY(y);
    updateTextPosition();
}

void UITextbox::setSize(float width, float height) {
    _size = raylib::rmath::Vector2(width, height);
    updateTextPosition();
}

bool UITextbox::isVisible() const { return _isVisible; }

void UITextbox::setVisible(bool visible) { _isVisible = visible; }

std::string UITextbox::getText() const { return _text; }

void UITextbox::setText(const std::string& text) {
    _text = text;
    updateTextPosition();
}

void UITextbox::setOnSubmit(std::function<void(const std::string&)> callback) { _onSubmit = std::move(callback); }

void UITextbox::updateTextPosition() {
    if (!_label) {
        return;
    }
    float const textY = _position.y() + ((_size.y() - DefaultFontSize) / 2.0F);
    _label->setPosition(_position.x() + TextPaddingX, textY);
}

}  // namespace zappy::gui::ui::components
