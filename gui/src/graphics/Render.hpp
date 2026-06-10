/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Render
*/

#pragma once

#include <memory>
#include <string>

#include "Map.hpp"
#include "SkyBackground.hpp"
#include "context/EventContext.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::graphics {
class Render {
  public:
    static constexpr const std::string WINDOW_NAME = "Zappy GUI";
    static constexpr int FLAG_FULLSCREEN_MODE = 2;

    Render() = default;
    ~Render() = default;
    Render(const Render& other) = delete;
    Render& operator=(const Render& other) = delete;
    Render(Render&& other) = delete;
    Render& operator=(Render&& other) = delete;

    void start();

  protected:
  private:
    void update();
    void render2D();
    void render3D();
    void handleEvents();
    raylib::rcore::Window _window{WINDOW_NAME.c_str(), FLAG_FULLSCREEN_MODE};
    std::shared_ptr<raylib::rcore::Camera> _camera{
        std::make_shared<raylib::rcore::Camera>(raylib::rmath::Vector3{10.0F, 10.0F, 10.0F})};
    EventContext _eventContext{_camera};
    scene::SkyBackground _skyBackground;
    raylib::rcore::Event _event;
    scene::Map _map{10, 10};  // TEMPORARY MAP SIZE, JUST FOR TESTING
};
}  // namespace zappy::gui::graphics
