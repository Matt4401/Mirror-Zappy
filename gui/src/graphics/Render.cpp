/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Render
*/

#include "Render.hpp"

#include <raylib.h>

#include <memory>
#include <utility>

#include "events/EventDispatcher.hpp"
#include "protocol/Commands.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"

namespace zappy::gui::graphics {
Render::Render(std::shared_ptr<events::EventDispatcher> dispatcher) : _dispatcher(std::move(dispatcher)) {
    if (_dispatcher) {
        _mszToken = _dispatcher->subscribe<shared::protocol::server::Msz>(
            [this](const shared::protocol::server::Msz& cmd) { _map.resize(cmd.width, cmd.height); });
    }
}

Render::~Render() {
    if (_dispatcher && _mszToken != 0) {
        _dispatcher->unsubscribe<shared::protocol::server::Msz>(_mszToken);
    }
}

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
    if (_camera->position().y() < 2.5F) {
        _camera->setPosition({_camera->position().x(), 2.5F, _camera->position().z()});
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
