/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** FirstPerson
*/

#pragma once

#include <functional>
#include <optional>

#include "events/EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "rcore/Camera.hpp"
#include "scene/WorldManager.hpp"

namespace zappy::gui::graphics {
class FirstPerson {
  public:
    static constexpr float EyeHeight = 3.2F;
    static constexpr float LookDistance = 8.0F;
    static constexpr float FovY = 70.0F;
    static constexpr int PerspectiveProjection = 0;

    FirstPerson(events::EventDispatcher& dispatcher, raylib::rcore::Camera& camera, scene::WorldManager& worldManager,
                std::function<void()> onStateChanged);
    ~FirstPerson();

    FirstPerson(const FirstPerson& other) = delete;
    FirstPerson& operator=(const FirstPerson& other) = delete;
    FirstPerson(FirstPerson&& other) = delete;
    FirstPerson& operator=(FirstPerson&& other) = delete;

    [[nodiscard]] bool active() const { return _playerId.has_value(); }
    void update();
    void exit();

  private:
    void handleRequest(const events::PlayerFirstPersonRequested& event);
    void updateCamera();
    void notifyStateChanged() const;

    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    std::reference_wrapper<raylib::rcore::Camera> _camera;
    std::reference_wrapper<scene::WorldManager> _worldManager;
    std::optional<int> _playerId;
    events::EventDispatcher::EventToken _eventToken{0};
    std::function<void()> _onStateChanged;
};
}  // namespace zappy::gui::graphics
