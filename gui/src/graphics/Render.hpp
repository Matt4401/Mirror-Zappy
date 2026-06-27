/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Render
*/

#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>

#include "FirstPerson.hpp"
#include "Map.hpp"
#include "Skybox3D.hpp"
#include "WorldManager.hpp"
#include "events/EventDispatcher.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Vector3.hpp"
#include "ui/UIManager.hpp"
#include "ui/hud/GameHUD.hpp"

namespace zappy::gui::graphics {
class Render {
  public:
    static constexpr const std::string WINDOW_NAME = "Zappy GUI";
    static constexpr int FLAG_FULLSCREEN_MODE = 2;

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
    void handleEscapeKey();
    void handleAltKey();
    void updateCameraLimits();

    std::map<int, std::function<void()>> _keyHandlers;
    raylib::rcore::Window _window{WINDOW_NAME.c_str()};
    raylib::rcore::Camera _camera{raylib::rmath::Vector3{10.0F, 10.0F, 10.0F}};
    scene::Skybox3D _skybox;
    raylib::rcore::Event _event;
    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    scene::WorldManager _worldManager;
    scene::Map _map;
    ui::UIManager _uiManager;
    std::shared_ptr<ui::hud::GameHUD> _gameHUD;
    std::unique_ptr<FirstPerson> _firstPerson;
    bool _isExiting{false};
    bool _uiMode{false};
    UpdateMode _updateMode{UpdateMode::All};

    static constexpr int EscapeKey = 256;
    static constexpr int LeftAltKey = 342;
    static constexpr int DefaultFps = 60;
    static constexpr std::string DefaultFontName = "Minecraft";
    static constexpr float MinCameraHeight = 1.3F;

    static constexpr float CameraPaddingMin = 50.0F;
    static constexpr float CameraPaddingMax = 60.0F;
    static constexpr float CameraHeightMin = 80.0F;
    static constexpr float CameraHeightMax = 110.0F;
    static constexpr float CameraPaddingFactor = 0.3F;
    static constexpr float CameraHeightFactor = 0.6F;
};
}  // namespace zappy::gui::graphics
