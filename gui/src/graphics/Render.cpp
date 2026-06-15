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
#include "game/Team.hpp"
#include "protocol/Commands.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "scene/Tile3D.hpp"

namespace zappy::gui::graphics {
Render::Render(std::shared_ptr<events::EventDispatcher> dispatcher) : _dispatcher(std::move(dispatcher)) {
    // std::string teamName = "Team1"; // TEMPORARY TEAM NAME, JUST FOR TESTING
    // _teams.emplace_back(game::Team(teamName, 5)); // TEMPORARY TEAM, JUST FOR TESTING
    // _teams[0].addPlayer({10.0F, scene::Tile3D::TILE_SIZE * 1.4, 0.0F}); // TEMPORARY PLAYER, JUST FOR TESTING
    if (_dispatcher) {
        _mszToken = _dispatcher->subscribe<shared::protocol::server::Msz>(
            [this](const shared::protocol::server::Msz& cmd) { _map.resize(cmd.width, cmd.height); });
    }
    _window.setTargetFPS(60);
}

Render::~Render() {
    if (_dispatcher && _mszToken != 0) {
        _dispatcher->unsubscribe<shared::protocol::server::Msz>(_mszToken);
    }
}

bool Render::isRunning() const { return !_window.shouldClose(); }

void Render::renderFrame() {
    update();
    _event.handleEvent(_eventContext);
    _window.beginDrawing();
    render2D();
    render3D();
    raylib::rcore::Window::endDrawing();
}

void Render::update() {
    _skyBackground.update(raylib::rcore::Window::frameTime());
    _camera->updateCamera(CAMERA_FREE);
    if (_camera->position().y() < scene::Tile3D::TILE_SIZE * 1.3F) {
        _camera->setPosition({_camera->position().x(), scene::Tile3D::TILE_SIZE * 1.3F, _camera->position().z()});
        _camera->setTarget({_camera->target().x(), _camera->target().y() + (raylib::rcore::Window::frameTime() * 2.0F),
                            _camera->target().z()});
    }
    _event.update();
}

void Render::render2D() { _skyBackground.draw(_window); }

void Render::render3D() {
    _camera->beginMode3D();
    _map.draw(*_camera);
    for (const auto& team : _teams) {
        team.draw(_gameModel);
    }
    raylib::rcore::Camera::endMode3D();
}
}  // namespace zappy::gui::graphics
