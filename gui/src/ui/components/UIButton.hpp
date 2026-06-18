/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIButton
*/

#pragma once

#include <functional>
#include <memory>
#include <string>

#include "Color.hpp"
#include "UIText.hpp"
#include "rcore/Event.hpp"
#include "rmath/Vector2.hpp"
#include "rtext/Font.hpp"
#include "ui/IUIComponent.hpp"

namespace zappy::gui::ui::components {

class UIButton : public IUIComponent {
  public:
    UIButton(float x, float y, float width, float height, const std::string& text,
             std::shared_ptr<raylib::rtext::Font> font);
    ~UIButton() override = default;

    UIButton(const UIButton& other) = delete;
    UIButton& operator=(const UIButton& other) = delete;
    UIButton(UIButton&& other) = delete;
    UIButton& operator=(UIButton&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent(const raylib::rcore::Event& event) override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;

    void setOnClick(std::function<void()> callback);
    void setFontSize(float size);

  private:
    void updateTextPosition();

    raylib::rmath::Vector2 _position;
    raylib::rmath::Vector2 _size;
    bool _isVisible{true};
    bool _isHovered{false};
    bool _isPressed{false};
    std::function<void()> _onClick;
    std::shared_ptr<raylib::rtext::Font> _fontRef;
    std::unique_ptr<UIText> _label;
    float _fontSize;

    static constexpr int MouseLeftButton = 0;
    static constexpr float DefaultFontSize = 20.0F;
    static constexpr float TextSpacing = 2.0F;
    static constexpr float PressedOffset = 2.0F;
    static constexpr float BorderThickness = 2.0F;
    static constexpr float OutlineThickness = 1.0F;

    static const raylib::Color OuterBorderColor;
    static const raylib::Color NormalFillColor;
    static const raylib::Color HoveredFillColor;
    static const raylib::Color PressedFillColor;
    static const raylib::Color BrightBorderColor;
    static const raylib::Color DarkBorderColor;
    static const raylib::Color HoverOutlineColor;
    static const raylib::Color TextHoverColor;
    static const raylib::Color TextNormalColor;
};

}  // namespace zappy::gui::ui::components
