/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** SkyBackground
*/

#pragma once

#include "Color.hpp"
#include "rcore/Window.hpp"
#include "rtextures/Texture2D.hpp"

namespace zappy::gui::graphics::scene {
class SkyBackground {
  public:
    static constexpr const char* TEXTURE_PATH = "assets/sky.png";
    static constexpr float DAY_SPEED = 0.112F;
    static constexpr float FULL_DAY = 1.0F;
    static constexpr raylib::Color NIGHT{38, 48, 92, 255};
    static constexpr raylib::Color SUNRISE{255, 184, 112, 255};
    static constexpr raylib::Color DAY{255, 255, 255, 255};
    static constexpr raylib::Color SUNSET{255, 151, 99, 255};

    SkyBackground() = default;
    ~SkyBackground() = default;
    SkyBackground(const SkyBackground& other) = delete;
    SkyBackground& operator=(const SkyBackground& other) = delete;
    SkyBackground(SkyBackground&& other) noexcept = default;
    SkyBackground& operator=(SkyBackground&& other) noexcept = default;

    void update(float deltaTime);
    void draw(raylib::rcore::Window& window) const;

  protected:
  private:
    [[nodiscard]] raylib::Color currentTint() const;
    [[nodiscard]] float horizontalPan() const;

    raylib::rtextures::Texture2D _texture{TEXTURE_PATH};
    float _dayProgress = 0.25F;
};
}  // namespace zappy::gui::graphics
