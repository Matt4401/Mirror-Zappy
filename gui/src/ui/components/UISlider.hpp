/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UISlider
*/

#pragma once

#include <functional>

#include "Color.hpp"
#include "rcore/Event.hpp"
#include "rmath/Vector2.hpp"
#include "ui/IUIComponent.hpp"

namespace zappy::gui::ui::components {

class UISlider : public IUIComponent {
  public:
    UISlider(float x, float y, float width, float height, float min, float max, float initialValue);
    UISlider(float x, float y, float width, float height, float min, float max, float initialValue,
             raylib::Color trackColor, raylib::Color handleNormalColor);
    ~UISlider() override = default;

    UISlider(const UISlider& other) = delete;
    UISlider& operator=(const UISlider& other) = delete;
    UISlider(UISlider&& other) = delete;
    UISlider& operator=(UISlider&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent(const raylib::rcore::Event& event) override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;
    [[nodiscard]] bool isHovered() const override { return _isHovered || _isDragging; }

    void setOnValueChanged(std::function<void(float)> callback);
    void setValue(float value);
    [[nodiscard]] float getValue() const;

  private:
    void updateHandlePositionFromValue();
    void updateValueFromHandlePosition();

    raylib::rmath::Vector2 _position;
    raylib::rmath::Vector2 _size;
    bool _isVisible{true};

    float _min;
    float _max;
    float _value;

    bool _isHovered{false};
    bool _isDragging{false};
    float _handleX{0.0F};

    std::function<void(float)> _onValueChanged;

    static constexpr int MouseLeftButton = 0;
    static constexpr float HandleWidthRatio = 0.05F;
    static constexpr float MinHandleWidth = 10.0F;

    raylib::Color _trackColor{80, 80, 80, 255};
    raylib::Color _handleNormalColor{150, 150, 150, 255};
    raylib::Color _handleHoverColor{180, 180, 180, 255};
    raylib::Color _handleDragColor{220, 220, 220, 255};
};

}  // namespace zappy::gui::ui::components
