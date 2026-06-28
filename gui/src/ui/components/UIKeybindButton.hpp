/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIKeybindButton
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

class UIKeybindButton : public IUIComponent {
  public:
    UIKeybindButton(float x, float y, float width, float height, std::string label, int initialKey,
                    std::shared_ptr<raylib::rtext::Font> font);
    ~UIKeybindButton() override = default;

    UIKeybindButton(const UIKeybindButton& other) = delete;
    UIKeybindButton& operator=(const UIKeybindButton& other) = delete;
    UIKeybindButton(UIKeybindButton&& other) = delete;
    UIKeybindButton& operator=(UIKeybindButton&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent() override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;
    [[nodiscard]] bool isHovered() const override;

    void setOnKeyBind(std::function<void(int)> callback);
    void setKey(int key);
    [[nodiscard]] int getKey() const;

  private:
    void updateTextPositions();
    void handleWaitingInput();
    [[nodiscard]] static std::string keyToString(int key);

    raylib::rmath::Vector2 _position;
    raylib::rmath::Vector2 _size;
    bool _isVisible{true};
    bool _isHovered{false};
    bool _isWaitingForInput{false};

    std::string _label;
    int _currentKey;
    std::shared_ptr<raylib::rtext::Font> _fontRef;

    raylib::rmath::Vector2 _labelPosition;
    raylib::rmath::Vector2 _buttonPosition;
    raylib::rmath::Vector2 _buttonSize;
    raylib::rmath::Vector2 _valuePosition;

    std::function<void(int)> _onKeyBind;

    static constexpr int MouseLeftButton = 0;
    static constexpr float DefaultFontSize = 20.0F;
    static constexpr float ButtonWidthRatio = 0.5F;

    static constexpr int MouseButtonOffset = 500;
    static constexpr int MaxMouseButtonOffset = 506;
    static constexpr int AsciiSpace = 32;
    static constexpr int AsciiTilde = 126;

    static const raylib::Color BackgroundColor;
    static const raylib::Color HoverColor;
    static const raylib::Color WaitingColor;
    static const raylib::Color BorderColor;
    static const raylib::Color TextColor;
};

}  // namespace zappy::gui::ui::components
