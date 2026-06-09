/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Render
*/

#include "Render.hpp"

#include <raylib.h>

#include "rcore/Camera.hpp"
#include "rcore/Window.hpp"

namespace zappy::gui::graphics {
void Render::start() {
    while (!_window.shouldClose()) {
        _camera.updateCamera(CAMERA_FIRST_PERSON);
        raylib::rcore::Window::beginDrawing();
        _window.clearBackground();
        render3D();
        raylib::rcore::Window::endDrawing();
    }
}

void Render::render3D() {
    _camera.beginMode3D();

    for (int x = -6; x <= 6; x += 2) {
        for (int z = -6; z <= 6; z += 2) {
            _tile.draw({static_cast<float>(x), 0.0F, static_cast<float>(z)});
        }
    }
    raylib::rcore::Camera::endMode3D();
}
}  // namespace zappy::gui::graphics
