/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UICompass
*/

#pragma once

#include <functional>
#include <memory>

#include "rcore/Camera.hpp"
#include "rtext/Font.hpp"
#include "ui/IUIComponent.hpp"

namespace zappy::gui::ui::components {

class UICompass : public IUIComponent {
  public:
    UICompass(float x, float y, float width, float height, raylib::rcore::Camera& camera,
              const std::shared_ptr<raylib::rtext::Font>& font);
    ~UICompass() override = default;

    UICompass(const UICompass& other) = delete;
    UICompass& operator=(const UICompass& other) = delete;
    UICompass(UICompass&& other) = delete;
    UICompass& operator=(UICompass&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent() override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;

    [[nodiscard]] bool isVisible() const override;
    [[nodiscard]] bool isHovered() const override;
    void setVisible(bool visible) override;

  private:
    float _x;
    float _y;
    float _width;
    float _height;
    bool _isVisible{true};

    std::reference_wrapper<raylib::rcore::Camera> _camera;
    std::shared_ptr<raylib::rtext::Font> _font;
};

}  // namespace zappy::gui::ui::components
