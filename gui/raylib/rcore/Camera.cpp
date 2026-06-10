/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Camera
*/

#include "rcore/Camera.hpp"

#include <raylib.h>

#include "rmath/Vector3.hpp"

namespace zappy::gui::raylib::rcore {
Camera::Camera(rmath::Vector3 position) : _camera{} {
    _camera.position = position.vector();
    _camera.target = rmath::Vector3(0, 0.0, 0);
    _camera.up = rmath::Vector3(0, 1.0, 0);
    _camera.fovy = 60.0;
    _camera.projection = CAMERA_PERSPECTIVE;
}

[[nodiscard]] bool Camera::isVisibleFromCamera(raylib::rmath::Vector3 position) const {
    const raylib::rmath::Vector3 toTile = position - static_cast<raylib::rmath::Vector3>(_camera.position);
    const float distanceSquared = toTile.lengthSquared();

    if (distanceSquared > MAX_RENDER_DISTANCE * MAX_RENDER_DISTANCE) {
        return false;
    }
    if (distanceSquared < MIN_DISTANCE * MIN_DISTANCE) {
        return true;
    }

    return direction().angle(toTile) <= halfFovyRadians(VIEW_PADDING_DEGREES);
}

void Camera::cameraPitch(float angle, bool lockView, bool rotateAroundTarget, bool rotateUp) {
    CameraPitch(&_camera, angle, lockView, rotateAroundTarget, rotateUp);
}
}  // namespace zappy::gui::raylib::rcore
