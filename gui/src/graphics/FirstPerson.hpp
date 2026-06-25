/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** FirstPerson
*/

#pragma once

#include <functional>
#include <memory>
#include <optional>

#include "events/EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "rcore/Camera.hpp"
#include "rtext/Font.hpp"
#include "scene/WorldManager.hpp"
#include "ui/UIManager.hpp"
#include "ui/components/UIButton.hpp"

namespace zappy::gui::graphics {
class FirstPerson {
  public:
    static constexpr float EyeHeight = 2.2F;
    static constexpr float LookDistance = 8.0F;
    static constexpr float FovY = 70.0F;
    static constexpr float ExitButtonWidth = 140.0F;
    static constexpr float ExitButtonHeight = 36.0F;
    static constexpr float ExitButtonTop = 16.0F;
    static constexpr float ExitButtonFontSize = 16.0F;
    static constexpr int PerspectiveProjection = 0;

    FirstPerson(events::EventDispatcher& dispatcher, raylib::rcore::Camera& camera, scene::WorldManager& worldManager,
                ui::UIManager& uiManager, const std::shared_ptr<raylib::rtext::Font>& font,
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
    void updateButtonPosition() const;
    void notifyStateChanged() const;

    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    std::reference_wrapper<raylib::rcore::Camera> _camera;
    std::reference_wrapper<scene::WorldManager> _worldManager;
    std::optional<int> _playerId;
    events::EventDispatcher::EventToken _eventToken{0};
    std::shared_ptr<ui::components::UIButton> _exitButton;
    std::function<void()> _onStateChanged;
};
}  // namespace zappy::gui::graphics
