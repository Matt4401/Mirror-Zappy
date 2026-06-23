/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIManager
*/

#include "UIManager.hpp"

#include <algorithm>
#include <memory>
#include <ranges>

#include "rcore/Event.hpp"
#include "ui/IUIComponent.hpp"

namespace zappy::gui::ui {

void UIManager::addComponent(const std::shared_ptr<IUIComponent>& component) {
    if (component) {
        _components.push_back(component);
    }
}

void UIManager::removeComponent(const std::shared_ptr<IUIComponent>& component) {
    auto it = std::ranges::find(_components, component);
    if (it != _components.end()) {
        _components.erase(it);
    }
}

void UIManager::clear() { _components.clear(); }

void UIManager::update() {
    for (auto& component : _components) {
        if (component->isVisible()) {
            component->update();
        }
    }
}

void UIManager::draw() {
    for (auto& component : _components) {
        if (component->isVisible()) {
            component->draw();
        }
    }
}

void UIManager::handleEvent(const raylib::rcore::Event& event) {
    for (auto& _component : std::ranges::reverse_view(_components)) {
        if (_component->isVisible()) {
            _component->handleEvent(event);
        }
    }
}

bool UIManager::isHovered() const {
    return std::ranges::any_of(_components,
                               [](const auto& component) { return component->isVisible() && component->isHovered(); });
}

}  // namespace zappy::gui::ui
