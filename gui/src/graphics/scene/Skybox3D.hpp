/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Skybox3D
*/

#pragma once

#include <functional>
#include <optional>

#include "Color.hpp"
#include "events/EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "rmodels/Model.hpp"
#include "rmodels/Shader.hpp"
#include "rtextures/Texture2D.hpp"

namespace zappy::gui::graphics::scene {
class Skybox3D {
  public:
    static constexpr float DAY_SPEED = 0.011F;
    static constexpr float FULL_DAY = 1.0F;
    static constexpr raylib::Color NIGHT{38, 48, 92, 255};
    static constexpr raylib::Color SUNRISE{255, 184, 112, 255};
    static constexpr raylib::Color DAY{255, 255, 255, 255};
    static constexpr raylib::Color SUNSET{255, 151, 99, 255};
    static constexpr float MORNING_START = 0.20F;
    static constexpr float AFTERNOON_START = 0.45F;
    static constexpr float EVENING_START = 0.72F;
    static constexpr float NIGHT_START = 0.88F;

    explicit Skybox3D(events::EventDispatcher& dispatcher);
    ~Skybox3D();
    Skybox3D(const Skybox3D& other) = delete;
    Skybox3D& operator=(const Skybox3D& other) = delete;
    Skybox3D(Skybox3D&& other) = delete;
    Skybox3D& operator=(Skybox3D&& other) = delete;

    void update(float deltaTime, float serverFrequency);
    void draw() const;

  protected:
  private:
    [[nodiscard]] raylib::Color currentTint() const;

    void loadCubemap();

    raylib::rmodels::Model _model;
    raylib::rmodels::Shader _shader;
    std::optional<raylib::rtextures::Texture2D> _cubemap;
    float _dayProgress{0.25F};

    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    events::EventDispatcher::EventToken _timeToken{0};
    events::TimeMode _timeMode{events::TimeMode::CYCLE};
};
}  // namespace zappy::gui::graphics::scene
