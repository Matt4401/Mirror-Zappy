/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** FirstPerson
*/

#include "FirstPerson.hpp"

#include <cmath>
#include <functional>
#include <utility>

#include "events/EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "rcore/Camera.hpp"
#include "rmath/Vector3.hpp"
#include "scene/WorldManager.hpp"

namespace zappy::gui::graphics {
FirstPerson::FirstPerson(events::EventDispatcher& dispatcher, raylib::rcore::Camera& camera,
                         scene::WorldManager& worldManager, std::function<void()> onStateChanged)
    : _dispatcher(dispatcher),
      _camera(camera),
      _worldManager(worldManager),
      _onStateChanged(std::move(onStateChanged)) {
    _eventToken = _dispatcher.get().subscribe<events::PlayerFirstPersonRequested>(
        [this](const events::PlayerFirstPersonRequested& event) { handleRequest(event); });
}

FirstPerson::~FirstPerson() {
    if (_eventToken != 0) {
        _dispatcher.get().unsubscribe<events::PlayerFirstPersonRequested>(_eventToken);
    }
}

void FirstPerson::handleRequest(const events::PlayerFirstPersonRequested& event) {
    if (!_worldManager.get().playerById(event.playerId).has_value()) {
        return;
    }

    _playerId = event.playerId;
    updateCamera();
    notifyStateChanged();
}

void FirstPerson::update() { updateCamera(); }

void FirstPerson::updateCamera() {
    if (!_playerId.has_value()) {
        return;
    }
    const auto player = _worldManager.get().playerById(*_playerId);
    if (!player.has_value()) {
        exit();
        return;
    }

    const auto direction = rotateDirectionAroundY(player->get().renderDirection(), PLAYER_DIRECTION_OFFSET.x());
    const auto eyePosition = player->get().position() + raylib::rmath::Vector3{0.0F, EyeHeight, 0.0F};

    _camera.get().setPosition(eyePosition);
    _camera.get().setTarget(eyePosition + (direction * LookDistance));
    _camera.get().setUp({0.0F, 1.0F, 0.0F});
    _camera.get().setFovy(FovY);
    _camera.get().setProjection(PerspectiveProjection);
}

raylib::rmath::Vector3 FirstPerson::rotateDirectionAroundY(const raylib::rmath::Vector3& direction,
                                                           const float angleDegrees) {
    const float angle = angleDegrees * raylib::rmath::Vector3::DegreesToRadians;
    const float cosine = std::cos(angle);
    const float sine = std::sin(angle);

    return {
        (direction.x() * cosine) + (direction.z() * sine),
        direction.y(),
        (-direction.x() * sine) + (direction.z() * cosine),
    };
}

void FirstPerson::exit() {
    if (!_playerId.has_value()) {
        return;
    }
    _playerId.reset();
    notifyStateChanged();
    _camera.get().setFovy(60.0F);
}

void FirstPerson::notifyStateChanged() const {
    if (_onStateChanged) {
        _onStateChanged();
    }
}
}  // namespace zappy::gui::graphics
