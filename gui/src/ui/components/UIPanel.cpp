/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIPanel
*/

#include "UIPanel.hpp"

#include <raylib.h>

#include <algorithm>
#include <memory>

#include "Color.hpp"
#include "rcore/Event.hpp"
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

    Rectangle const rec = {_position.x(), _position.y(), _size.x(), _size.y()};
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

void UIPanel::handleEvent(const raylib::rcore::Event& event) {
    if (!_isVisible) {
        return;
    }
    for (auto it = _children.rbegin(); it != _children.rend(); ++it) {
        if ((*it)->isVisible()) {
            (*it)->handleEvent(event);
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

void UIPanel::addComponent(const std::shared_ptr<IUIComponent>& component) {
    if (component) {
        _children.emplace_back(component);
    }
}

void UIPanel::removeComponent(const std::shared_ptr<IUIComponent>& component) {
    auto it = std::ranges::find(_children, component);
    if (it != _children.end()) {
        _children.erase(it);
    }
}

void UIPanel::setRounded(bool rounded) { _isRounded = rounded; }

}  // namespace zappy::gui::ui::components
