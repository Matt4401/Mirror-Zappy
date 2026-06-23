/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UITextbox
*/

#pragma once

#include <functional>
#include <memory>
#include <string>

#include "Color.hpp"
#include "rcore/Event.hpp"
#include "rmath/Vector2.hpp"
#include "rtext/Font.hpp"
#include "ui/IUIComponent.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::components {

class UITextbox : public IUIComponent {
  public:
    UITextbox(float x, float y, float width, float height, const std::shared_ptr<raylib::rtext::Font>& font,
              std::string placeholder = "");
    ~UITextbox() override = default;

    UITextbox(const UITextbox& other) = delete;
    UITextbox& operator=(const UITextbox& other) = delete;
    UITextbox(UITextbox&& other) = delete;
    UITextbox& operator=(UITextbox&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent(const raylib::rcore::Event& event) override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;

    [[nodiscard]] std::string getText() const;
    void setText(const std::string& text);

    void setOnSubmit(std::function<void(const std::string&)> callback);

  private:
    void updateTextPosition();
    void updateHoverState();
    void handleInput();

    raylib::rmath::Vector2 _position;
    raylib::rmath::Vector2 _size;
    bool _isVisible{true};

    std::string _text;
    std::string _placeholder;
    bool _isFocused{false};
    bool _isHovered{false};

    std::unique_ptr<UIText> _label;
    std::shared_ptr<raylib::rtext::Font> _fontRef;

    std::function<void(const std::string&)> _onSubmit;

    static constexpr float DefaultFontSize = 16.0F;
    static constexpr float TextPaddingX = 8.0F;
    static constexpr int MouseLeftButton = 0;
    static constexpr int KeyBackspace = 259;
    static constexpr int KeyEnter = 257;
    static constexpr int KeyKpEnter = 335;
    static constexpr int KeyMinusA = 45;
    static constexpr int KeyMajorZ = 90;

    static constexpr raylib::Color BackgroundColor{40, 40, 40, 255};
    static constexpr raylib::Color BorderNormalColor{100, 100, 100, 255};
    static constexpr raylib::Color BorderFocusedColor{150, 150, 255, 255};
    static constexpr raylib::Color TextColor{255, 255, 255, 255};
    static constexpr raylib::Color PlaceholderColor{120, 120, 120, 255};
};

}  // namespace zappy::gui::ui::components
