/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Render
*/

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>

#include "Map.hpp"
#include "Skybox3D.hpp"
#include "WorldManager.hpp"
#include "events/EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Vector3.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/UIManager.hpp"
#include "ui/hud/GameHUD.hpp"

namespace zappy::gui::graphics {
class Render {
  public:
    static constexpr const std::string WINDOW_NAME = "Zappy GUI";
    static constexpr int FLAG_FULLSCREEN_MODE = 2;
    static constexpr float FirstPersonEyeHeight = 2.2F;
    static constexpr float FirstPersonLookDistance = 8.0F;
    static constexpr float FirstPersonFovY = 70.0F;
    static constexpr float ExitViewButtonWidth = 140.0F;
    static constexpr float ExitViewButtonHeight = 36.0F;
    static constexpr float ExitViewButtonTop = 16.0F;
    static constexpr float ExitViewButtonFontSize = 16.0F;
    static constexpr int PerspectiveProjection = 0;

    explicit Render(events::EventDispatcher& dispatcher);
    ~Render();
    Render(const Render& other) = delete;
    Render& operator=(const Render& other) = delete;
    Render(Render&& other) = delete;
    Render& operator=(Render&& other) = delete;

    [[nodiscard]] bool isRunning() const;
    void renderFrame();

  protected:
  private:
    enum class UpdateMode : uint8_t {
        All,
        PauseMenuOnly,
    };

    void update();
    void render2D();
    void render3D();
    void updateCursorState() const;
    void handleInput();
    void handleFirstPersonRequest(const events::PlayerFirstPersonRequested& event);
    void updateFirstPersonCamera();
    void exitFirstPersonView();
    void updateExitFirstPersonButtonPosition() const;

    raylib::rcore::Window _window{WINDOW_NAME.c_str()};
    raylib::rcore::Camera _camera{raylib::rmath::Vector3{10.0F, 10.0F, 10.0F}};
    scene::Skybox3D _skybox;
    raylib::rcore::Event _event;
    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    scene::WorldManager _worldManager;
    scene::Map _map;
    ui::UIManager _uiManager;
    std::shared_ptr<ui::hud::GameHUD> _gameHUD;
    bool _isExiting{false};
    bool _uiMode{false};
    events::EventDispatcher::EventToken _firstPersonToken{0};
    std::optional<int> _firstPersonPlayerId;
    std::shared_ptr<ui::components::UIButton> _exitFirstPersonButton;
    UpdateMode _updateMode{UpdateMode::All};

    static constexpr int EscapeKey = 256;
    static constexpr int LeftAltKey = 342;
    static constexpr int DefaultFps = 60;
    static constexpr std::string DefaultFontName = "Minecraft";
};
}  // namespace zappy::gui::graphics
