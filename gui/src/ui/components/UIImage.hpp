/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIImage
*/

#pragma once

#include <memory>
#include <string>

#include "Color.hpp"
#include "rmath/Vector2.hpp"
#include "rtextures/Texture2D.hpp"
#include "ui/IUIComponent.hpp"

namespace zappy::gui::ui::components {

class UIImage : public IUIComponent {
  public:
    explicit UIImage(const std::string& path);
    ~UIImage() override = default;

    UIImage(const UIImage& other) = delete;
    UIImage& operator=(const UIImage& other) = delete;
    UIImage(UIImage&& other) = delete;
    UIImage& operator=(UIImage&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent() override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;
    [[nodiscard]] bool isHovered() const override { return false; }

    void setTint(raylib::Color color);
    void setScale(float scale);
    void setTexture(const std::string& path);
    [[nodiscard]] std::string getPath() const { return _path; }

  private:
    std::shared_ptr<raylib::rtextures::Texture2D> _texture;
    std::string _path;
    raylib::rmath::Vector2 _position{0.0F, 0.0F};
    raylib::rmath::Vector2 _size{0.0F, 0.0F};
    raylib::Color _tint{raylib::Color::White()};
    float _scale{1.0F};
    bool _isVisible{true};
};

}  // namespace zappy::gui::ui::components
