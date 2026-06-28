/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIToggle
*/

#pragma once

#include <functional>
#include <memory>
#include <string>

#include "Color.hpp"
#include "rmath/Vector2.hpp"
#include "rtext/Font.hpp"
#include "ui/IUIComponent.hpp"

namespace zappy::gui::ui::components {

class UIToggle : public IUIComponent {
  public:
    UIToggle(float x, float y, float width, float height, std::string text, std::shared_ptr<raylib::rtext::Font> font,
             bool initialState = false);
    ~UIToggle() override = default;

    UIToggle(const UIToggle& other) = delete;
    UIToggle& operator=(const UIToggle& other) = delete;
    UIToggle(UIToggle&& other) = delete;
    UIToggle& operator=(UIToggle&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent() override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;
    [[nodiscard]] bool isHovered() const override;

    void setOnToggle(std::function<void(bool)> callback);
    void setState(bool state);
    [[nodiscard]] bool getState() const;

  private:
    void updateTextPosition();

    raylib::rmath::Vector2 _position;
    raylib::rmath::Vector2 _size;
    bool _isVisible{true};
    bool _isHovered{false};
    bool _state{false};

    std::string _text;
    std::shared_ptr<raylib::rtext::Font> _fontRef;
    raylib::rmath::Vector2 _textPosition;

    std::function<void(bool)> _onToggle;

    static constexpr int MouseLeftButton = 0;
    static constexpr float DefaultFontSize = 20.0F;
    static constexpr float ToggleBoxSize = 24.0F;
    static constexpr float TextPaddingX = 10.0F;

    static const raylib::Color BackgroundColor;
    static const raylib::Color HoverColor;
    static const raylib::Color BorderColor;
    static const raylib::Color TextColor;
    static const raylib::Color CheckColor;
};

}  // namespace zappy::gui::ui::components
