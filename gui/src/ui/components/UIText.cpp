/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIText
*/

#include "UIText.hpp"

#include <memory>
#include <string>
#include <utility>

#include "Color.hpp"
#include "rmath/Vector2.hpp"
#include "rtext/Font.hpp"
#include "rtext/Text.hpp"

namespace zappy::gui::ui::components {

UIText::UIText(std::string text, std::shared_ptr<raylib::rtext::Font> font)
    : _text(std::move(text)), _font(std::move(font)) {
    if (_font && _font->valid()) {
        _fontSize = static_cast<float>(_font->baseSize());
    }
}

void UIText::draw() {
    if (!_isVisible || _text.empty() || !_font || !_font->valid()) {
        return;
    }
    raylib::rtext::Text const textObj(_text, _position);
    textObj.draw(*_font, _fontSize, _spacing, _color);
}

void UIText::update() {}

void UIText::handleEvent(const raylib::rcore::Event& /*event*/) {}

void UIText::setPosition(float x, float y) { _position = raylib::rmath::Vector2(x, y); }

void UIText::setSize(float /*width*/, float /*height*/) {}

bool UIText::isVisible() const { return _isVisible; }

void UIText::setVisible(bool visible) { _isVisible = visible; }

void UIText::setText(const std::string& text) { _text = text; }

void UIText::setFontSize(float size) { _fontSize = size; }

void UIText::setColor(raylib::Color color) { _color = color; }

void UIText::setSpacing(float spacing) { _spacing = spacing; }

float UIText::getWidth() const {
    if (_text.empty() || !_font || !_font->valid()) {
        return 0.0F;
    }
    return raylib::rtext::Text::measureText(*_font, _text, _fontSize, _spacing).x();
}

}  // namespace zappy::gui::ui::components
