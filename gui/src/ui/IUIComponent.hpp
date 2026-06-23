/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** IUIComponent
*/

#pragma once

namespace zappy::gui::ui {

class IUIComponent {
  public:
    IUIComponent() = default;
    virtual ~IUIComponent() = default;

    IUIComponent& operator=(const IUIComponent& other) = delete;
    IUIComponent(const IUIComponent& other) = delete;
    IUIComponent& operator=(IUIComponent&& other) = delete;
    IUIComponent(IUIComponent&& other) = delete;

    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void handleEvent() = 0;

    virtual void setPosition(float x, float y) = 0;
    virtual void setSize(float width, float height) = 0;
    [[nodiscard]] virtual bool isVisible() const = 0;
    virtual void setVisible(bool visible) = 0;
    [[nodiscard]] virtual bool isHovered() const = 0;
};

}  // namespace zappy::gui::ui
