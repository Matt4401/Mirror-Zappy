/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Render
*/

#include "Render.hpp"

#include <raylib.h>

#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"

namespace zappy::gui::graphics {
void Render::start() {
    while (!_window.shouldClose()) {
        update();
        _event.handleEvent(_eventContext);
        _window.beginDrawing();
        render2D();
        render3D();
        raylib::rcore::Window::endDrawing();
    }
}

void Render::update() {
    _skyBackground.update(raylib::rcore::Window::frameTime());
    _camera->updateCamera(CAMERA_FREE);
    if (_camera->position().y() < 4.0F) {
        _camera->setPosition({_camera->position().x(), 4.0F, _camera->position().z()});
        _camera->setTarget({_camera->target().x(), _camera->target().y() + (raylib::rcore::Window::frameTime() * 2.0F),
                            _camera->target().z()});
    }
    _event.update();
}

void Render::render2D() { _skyBackground.draw(_window); }

void Render::render3D() {
    _camera->beginMode3D();
    _map.draw(*_camera);
    raylib::rcore::Camera::endMode3D();
}
}  // namespace zappy::gui::graphics
