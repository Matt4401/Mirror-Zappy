/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Camera
*/

#pragma once
#include <raylib.h>

#include "rcamera.h"

namespace zappy::gui::raylib::rcore {
class Camera {
  public:
    Camera(Vector3 position) : _camera{} { _camera.position = position; }
    Camera() = default;
    ~Camera() = default;
    Camera(const Camera& other) = delete;
    Camera& operator=(const Camera& other) = delete;
    Camera(Camera&& other) = delete;
    Camera& operator=(Camera&& other) = delete;

    static void beginMode3D(Camera3D camera) { BeginMode3D(camera); }
    static void endMode3D() { EndMode3D(); }

    [[nodiscard]] Camera3D camera() const { return _camera; }

    void setPosition(Vector3 position) { _camera.position = position; }
    void setTarget(Vector3 target) { _camera.target = target; }
    void setUp(Vector3 up) { _camera.up = up; }
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
