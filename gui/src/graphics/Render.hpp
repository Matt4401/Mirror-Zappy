/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Render
*/

#pragma once
#include <string>

#include "Tile3D.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Window.hpp"

namespace zappy::gui::graphics {
class Render {
  public:
    static constexpr const std::string WINDOW_NAME = "Zappy GUI";

    Render() = default;
    ~Render() = default;
    Render(const Render& other) = delete;
    Render& operator=(const Render& other) = delete;
    Render(Render&& other) = delete;
    Render& operator=(Render&& other) = delete;

    void start();

  protected:
  private:
    void render3D();
    raylib::rcore::Window _window{WINDOW_NAME.c_str()};
    raylib::rcore::Camera _camera{{10.0F, 10.0F, 10.0F}};
    Tile3D _tile;
};
}  // namespace zappy::gui::graphics
