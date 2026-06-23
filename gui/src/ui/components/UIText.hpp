/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIText
*/

#pragma once

#include <memory>
#include <string>

#include "Color.hpp"
#include "rcore/Event.hpp"
#include "rmath/Vector2.hpp"
#include "rtext/Font.hpp"
#include "ui/IUIComponent.hpp"

namespace zappy::gui::ui::components {

class UIText : public IUIComponent {
  public:
    UIText(std::string text, std::shared_ptr<raylib::rtext::Font> font);
    ~UIText() override = default;

    UIText(const UIText& other) = delete;
    UIText& operator=(const UIText& other) = delete;
    UIText(UIText&& other) = delete;
    UIText& operator=(UIText&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent(const raylib::rcore::Event& event) override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;
    [[nodiscard]] bool isHovered() const override { return false; }

    void setText(const std::string& text);
    void setFontSize(float size);
    void setColor(raylib::Color color);
    void setSpacing(float spacing);
    [[nodiscard]] const std::string& text() const { return _text; }
    [[nodiscard]] float getWidth() const;

  private:
    std::string _text;
    std::shared_ptr<raylib::rtext::Font> _font;
    raylib::rmath::Vector2 _position{0.0F, 0.0F};
    float _fontSize{20.0F};
    float _spacing{2.0F};
    raylib::Color _color{raylib::Color::Black()};
    bool _isVisible{true};
};

}  // namespace zappy::gui::ui::components
