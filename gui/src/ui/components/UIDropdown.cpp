/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIDropdown
*/

#include "UIDropdown.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "Color.hpp"
#include "components/UIText.hpp"
#include "rcore/Event.hpp"
#include "rmath/Rectangle.hpp"
#include "rmath/Vector2.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"

namespace zappy::gui::ui::components {

using Shapes = raylib::rshapes::Shapes;

const raylib::Color UIDropdown::BackgroundColor{40, 40, 40, 255};
const raylib::Color UIDropdown::HoverColor{70, 70, 70, 255};
const raylib::Color UIDropdown::BorderColor{100, 100, 100, 255};
const raylib::Color UIDropdown::TextColor{255, 255, 255, 255};

UIDropdown::UIDropdown(float x, float y, float width, float height, std::vector<std::string> options,
                       std::shared_ptr<raylib::rtext::Font> font)
    : _position(x, y), _size(width, height), _options(std::move(options)), _fontRef(std::move(font)) {}

void UIDropdown::draw() {
    if (!_isVisible) {
        return;
    }

    raylib::rmath::Rectangle const mainRec{
        .x = _position.x(), .y = _position.y(), .width = _size.x(), .height = _size.y()};
    Shapes::drawRectangleRec(mainRec, _isMainHovered ? HoverColor : BackgroundColor);
    Shapes::drawRectangleLinesEx(mainRec, 2.0F, BorderColor);

    float const textYOffset = (_size.y() - DefaultFontSize) / 2.0F;

    if (!_options.empty() && _selectedIndex < _options.size()) {
        UIText mainText(_options.at(_selectedIndex), _fontRef);
        mainText.setFontSize(static_cast<int>(DefaultFontSize));
        mainText.setColor(TextColor);
        mainText.setPosition(_position.x() + TextPaddingX, _position.y() + textYOffset);
        mainText.draw();
    }

    if (_isOpen) {
        float currentY = _position.y();
        if (_direction == Direction::DOWN) {
            currentY += _size.y();
        } else {
            currentY -= _size.y() * static_cast<float>(_options.size());
        }

        for (size_t i = 0; i < _options.size(); ++i) {
            raylib::rmath::Rectangle const itemRec{
                .x = _position.x(), .y = currentY, .width = _size.x(), .height = _size.y()};

            bool const isHovered = (_hoveredIndex.has_value() && i == _hoveredIndex.value());
            Shapes::drawRectangleRec(itemRec, isHovered ? HoverColor : BackgroundColor);
            Shapes::drawRectangleLinesEx(itemRec, 1.0F, BorderColor);

            UIText itemText(_options.at(i), _fontRef);
            itemText.setFontSize(static_cast<int>(DefaultFontSize));
            itemText.setColor(TextColor);
            itemText.setPosition(_position.x() + TextPaddingX, currentY + textYOffset);
            itemText.draw();

            currentY += _size.y();
        }
    }
}

void UIDropdown::update() {
    if (!_isVisible) {
        return;
    }

    raylib::rmath::Vector2 const mousePos = raylib::rcore::Event::getMousePositionStatic();

    raylib::rmath::Rectangle const mainRec{
        .x = _position.x(), .y = _position.y(), .width = _size.x(), .height = _size.y()};
    _isMainHovered = (mousePos.x() >= mainRec.x && mousePos.x() <= mainRec.x + mainRec.width &&
                      mousePos.y() >= mainRec.y && mousePos.y() <= mainRec.y + mainRec.height);

    _hoveredIndex = std::nullopt;
    if (_isOpen) {
        float startY = _position.y();
        if (_direction == Direction::DOWN) {
            startY += _size.y();
        } else {
            startY -= _size.y() * static_cast<float>(_options.size());
        }

        for (size_t i = 0; i < _options.size(); ++i) {
            float const currentY = startY + (static_cast<float>(i) * _size.y());
            raylib::rmath::Rectangle const itemRec{
                .x = _position.x(), .y = currentY, .width = _size.x(), .height = _size.y()};
            if (mousePos.x() >= itemRec.x && mousePos.x() <= itemRec.x + itemRec.width && mousePos.y() >= itemRec.y &&
                mousePos.y() <= itemRec.y + itemRec.height) {
                _hoveredIndex = i;
                break;
            }
        }
    }
}

void UIDropdown::handleEvent(const raylib::rcore::Event& event) {
    (void)event;
    if (!_isVisible) {
        return;
    }

    if (raylib::rcore::Event::isMouseButtonReleased(MouseLeftButton)) {
        if (_isOpen) {
            if (_hoveredIndex.has_value()) {
                _selectedIndex = _hoveredIndex.value();
                if (_onSelect) {
                    _onSelect(_options.at(_selectedIndex));
                }
                _isOpen = false;
            } else {
                _isOpen = false;
            }
        } else {
            if (_isMainHovered) {
                _isOpen = true;
            }
        }
    }
}

void UIDropdown::setPosition(float x, float y) {
    _position.setX(x);
    _position.setY(y);
}

void UIDropdown::setSize(float width, float height) {
    _size.setX(width);
    _size.setY(height);
}

bool UIDropdown::isVisible() const { return _isVisible; }

void UIDropdown::setVisible(bool visible) { _isVisible = visible; }

void UIDropdown::setOnSelect(std::function<void(const std::string&)> callback) { _onSelect = std::move(callback); }

void UIDropdown::setDirection(Direction dir) { _direction = dir; }

void UIDropdown::setSelectedIndex(size_t index) {
    if (index < _options.size()) {
        _selectedIndex = index;
    }
}

size_t UIDropdown::getSelectedIndex() const { return _selectedIndex; }

std::string UIDropdown::getSelectedOption() const {
    if (_options.empty() || _selectedIndex >= _options.size()) {
        return "";
    }
    return _options.at(_selectedIndex);
}

}  // namespace zappy::gui::ui::components
