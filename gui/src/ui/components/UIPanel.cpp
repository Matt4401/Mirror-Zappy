/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIPanel
*/

#include "UIPanel.hpp"

#include <algorithm>
#include <memory>
#include <ranges>
#include <vector>

#include "Color.hpp"
#include "rcore/Event.hpp"
#include "rmath/Rectangle.hpp"
#include "rmath/Vector2.hpp"
#include "rshapes/Shapes.hpp"
#include "ui/IUIComponent.hpp"

namespace zappy::gui::ui::components {

UIPanel::UIPanel(float x, float y, float width, float height, raylib::Color color)
    : _position(x, y), _size(width, height), _color(color) {}

void UIPanel::draw() {
    if (!_isVisible) {
        return;
    }

    raylib::rmath::Rectangle const rec{.x = _position.x(), .y = _position.y(), .width = _size.x(), .height = _size.y()};
    if (_isRounded) {
        raylib::rshapes::Shapes::drawRectangleRounded(rec, RoundednessFactor, RoundedSegments, _color);
    } else {
        raylib::rshapes::Shapes::drawRectangleRec(rec, _color);
    }

    for (auto& child : _children) {
        if (child->isVisible()) {
            child->draw();
        }
    }
}

void UIPanel::update() {
    if (!_isVisible) {
        return;
    }
    for (auto& child : _children) {
        if (child->isVisible()) {
            child->update();
        }
    }
}

void UIPanel::handleEvent() {
    if (!_isVisible) {
        return;
    }
    for (auto& child : std::views::reverse(_children)) {
        if (child->isVisible()) {
            child->handleEvent();
        }
    }
}

void UIPanel::setPosition(float x, float y) {
    _position.setX(x);
    _position.setY(y);
}

void UIPanel::setSize(float width, float height) { _size = raylib::rmath::Vector2(width, height); }

bool UIPanel::isVisible() const { return _isVisible; }

void UIPanel::setVisible(bool visible) { _isVisible = visible; }

bool UIPanel::isHovered() const {
    if (!_isVisible) {
        return false;
    }
    raylib::rmath::Vector2 const mousePos = raylib::rcore::Event::getMousePositionStatic();
    raylib::rmath::Rectangle const rec{.x = _position.x(), .y = _position.y(), .width = _size.x(), .height = _size.y()};
    if (raylib::rshapes::Shapes::checkCollisionPointRec(mousePos, rec)) {
        return true;
    }
    return std::ranges::any_of(_children, [](const auto& child) { return child->isHovered(); });
}

void UIPanel::addComponent(const std::shared_ptr<IUIComponent>& component) {
    if (component && std::ranges::find(_children, component) == _children.end()) {
        _children.emplace_back(component);
    }
}

void UIPanel::removeComponent(const std::shared_ptr<IUIComponent>& component) {
    if (component) {
        std::erase(_children, component);
    }
}

void UIPanel::setRounded(bool rounded) { _isRounded = rounded; }

}  // namespace zappy::gui::ui::components
