/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Camera
*/

#pragma once
#include <raylib.h>

#include "rcamera.h"
#include "rmath/Vector3.hpp"

namespace zappy::gui::raylib::rcore {
class Camera {
  public:
    Camera(rmath::Vector3 position) : _camera{} { _camera.position = position.vector();
    _camera.target = rmath::Vector3(0, 0.0, 0); _camera.up = rmath::Vector3(0, 1.0, 0); _camera.fovy = 60.0; _camera.projection = CAMERA_PERSPECTIVE; }
    ~Camera() = default;
    Camera(const Camera& other) = delete;
    Camera& operator=(const Camera& other) = delete;
    Camera(Camera&& other) = delete;
    Camera& operator=(Camera&& other) = delete;

    [[nodiscard]] Camera3D camera() const { return _camera; }

    void beginMode3D() const { BeginMode3D(_camera); }
    static void endMode3D() { EndMode3D(); }

    void setPosition(rmath::Vector3 position) { _camera.position = position.vector(); }
    void setTarget(rmath::Vector3 target) { _camera.target = target.vector(); }
    void setUp(rmath::Vector3 up) { _camera.up = up.vector(); }
    void setFovy(float fovy) { _camera.fovy = fovy; }
    void setProjection(int projection) { _camera.projection = projection; }

    void updateCamera(int mode) { UpdateCamera(&_camera, mode); }

    void cameraYaw(float angle, bool rotateAroundTarget) { CameraYaw(&_camera, angle, rotateAroundTarget); }
    void cameraPitch(float angle, bool lockView, bool rotateAroundTarget, bool rotateUp) {
        CameraPitch(&_camera, angle, lockView, rotateAroundTarget, rotateUp);
    }

  protected:
  private:
    Camera3D _camera;
};
}  // namespace zappy::gui::raylib::rcore
