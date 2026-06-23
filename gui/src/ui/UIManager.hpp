/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIManager
*/

#pragma once

#include <memory>
#include <vector>

#include "IUIComponent.hpp"
#include "rcore/Event.hpp"

namespace zappy::gui::ui {

class UIManager {
  public:
    UIManager() = default;
    ~UIManager() = default;

    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;
    UIManager(UIManager&&) = delete;
    UIManager& operator=(UIManager&&) = delete;

    void addComponent(const std::shared_ptr<IUIComponent>& component);
    void removeComponent(const std::shared_ptr<IUIComponent>& component);
    void clear();

    void update();
    void draw();
    void handleEvent(const raylib::rcore::Event& event);

    [[nodiscard]] bool isHovered() const;

  private:
    std::vector<std::shared_ptr<IUIComponent>> _components;
};

}  // namespace zappy::gui::ui
