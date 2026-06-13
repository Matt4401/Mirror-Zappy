/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIPanel
*/

#pragma once

#include <memory>
#include <vector>

#include "../IUIComponent.hpp"
#include "Color.hpp"
#include "rcore/Event.hpp"
#include "rmath/Vector2.hpp"

namespace zappy::gui::ui::components {

class UIPanel : public IUIComponent {
  public:
    UIPanel(float x, float y, float width, float height, raylib::Color color = raylib::Color::White());
    ~UIPanel() override = default;

    UIPanel(const UIPanel& other) = delete;
    UIPanel& operator=(const UIPanel& other) = delete;
    UIPanel(UIPanel&& other) = delete;
    UIPanel& operator=(UIPanel&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent(const raylib::rcore::Event& event) override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;

    void setRounded(bool rounded);

    void addComponent(const std::shared_ptr<IUIComponent>& component);
    void removeComponent(const std::shared_ptr<IUIComponent>& component);

  private:
    raylib::rmath::Vector2 _position;
    raylib::rmath::Vector2 _size;
    raylib::Color _color;
    bool _isVisible{true};
    bool _isRounded{true};
    std::vector<std::shared_ptr<IUIComponent>> _children;

    static constexpr float RoundednessFactor = 0.2F;
    static constexpr int RoundedSegments = 10;
};

}  // namespace zappy::gui::ui::components
