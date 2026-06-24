/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** EventLogRow
*/

#include "EventLogRow.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Color.hpp"
#include "components/UIText.hpp"
#include "rtext/Font.hpp"

namespace zappy::gui::ui::components {

EventLogRow::EventLogRow(const std::vector<std::pair<std::string, raylib::Color>>& segments,
                         const std::shared_ptr<raylib::rtext::Font>& font) {
    for (const auto& [text, color] : segments) {
        auto uiText = std::make_shared<components::UIText>(text, font);
        uiText->setColor(color);
        uiText->setFontSize(FontSize);
        _texts.push_back(uiText);
    }
}

void EventLogRow::draw() {
    for (auto& t : _texts) {
        if (t->isVisible()) {
            t->draw();
        }
    }
}

void EventLogRow::update() {
    for (auto& t : _texts) {
        if (t->isVisible()) {
            t->update();
        }
    }
}

void EventLogRow::handleEvent() {}

void EventLogRow::setPosition(float x, float y, float width) {
    _position.setX(x);
    _position.setY(y);

    float currentX = x;
    float currentY = y;
    for (auto& t : _texts) {
        if (width > 0.0F && currentX + t->getWidth() > x + width && currentX > x) {
            currentX = x;
            currentY += FontSize + 4.0F;
        }
        t->setPosition(currentX, currentY);
        currentX += t->getWidth();
    }
}

void EventLogRow::setPosition(float x, float y) { setPosition(x, y, _size.x()); }

void EventLogRow::setSize(float width, float height) {
    _size.setX(width);
    _size.setY(height);
    setPosition(_position.x(), _position.y(), width);
}

bool EventLogRow::isVisible() const { return _isVisible; }

void EventLogRow::setVisible(bool visible) {
    _isVisible = visible;
    for (auto& t : _texts) {
        t->setVisible(visible);
    }
}

bool EventLogRow::isHovered() const { return false; }

}  // namespace zappy::gui::ui::components
