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
    static constexpr float MAX_RENDER_DISTANCE = 160.0F;
    static constexpr float VIEW_PADDING_DEGREES = 35.0F;
    static constexpr float MIN_DISTANCE = 2.0F;
    static constexpr float VISIBILITY_ORIGIN_BACKSTEP = 4.0F;

    Camera() : _camera{} {}
    Camera(rmath::Vector3 position);
    Camera(rmath::Vector3 position, rmath::Vector3 target, rmath::Vector3 up, float fovy, int projection)
        : _camera{.position = position.vector(),
                  .target = target.vector(),
                  .up = up.vector(),
                  .fovy = fovy,
                  .projection = projection} {}
    ~Camera() = default;
    Camera(const Camera& other) = delete;
    Camera& operator=(const Camera& other) = delete;
    Camera(Camera&& other) noexcept = default;
    Camera& operator=(Camera&& other) noexcept = default;

    [[nodiscard]] Camera3D camera() const { return _camera; }
    [[nodiscard]] rmath::Vector3 position() const { return _camera.position; }
    [[nodiscard]] rmath::Vector3 target() const { return _camera.target; }
    [[nodiscard]] rmath::Vector3 up() const { return _camera.up; }
    [[nodiscard]] float fovy() const { return _camera.fovy; }
    [[nodiscard]] rmath::Vector3 direction() const { return (target() - position()).normalized(); }
    [[nodiscard]] float halfFovyRadians(float paddingDegrees = 0.0F) const {
        return ((_camera.fovy * 0.5F) + paddingDegrees) * DEG2RAD;
    }

    void beginMode3D() const { BeginMode3D(_camera); }
    static void endMode3D() { EndMode3D(); }

    void setPosition(rmath::Vector3 position) { _camera.position = position.vector(); }
    void setTarget(rmath::Vector3 target) { _camera.target = target.vector(); }
    void setUp(rmath::Vector3 up) { _camera.up = up.vector(); }
    void setFovy(float fovy) { _camera.fovy = fovy; }
    void setProjection(int projection) { _camera.projection = projection; }

    void updateCamera(int mode) { UpdateCamera(&_camera, mode); }

    void cameraYaw(float angle, bool rotateAroundTarget) { CameraYaw(&_camera, angle, rotateAroundTarget); }
    void cameraPitch(float angle, bool lockView, bool rotateAroundTarget, bool rotateUp);

    [[nodiscard]] bool isVisibleFromCamera(raylib::rmath::Vector3 position) const;

  protected:
  private:
    Camera3D _camera;
};
}  // namespace zappy::gui::raylib::rcore
