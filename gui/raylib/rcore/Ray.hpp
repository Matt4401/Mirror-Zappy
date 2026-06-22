/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Ray
*/

#pragma once

#include <raylib.h>

#include "rcore/BoundingBox.hpp"

namespace zappy::gui::raylib::rcore {
struct RayCollision {
    bool hit{false};
    float distance{0.0F};
};

class Ray {
  public:
    explicit Ray(::Ray ray) : _ray(ray) {}

    [[nodiscard]] RayCollision collision(const BoundingBox& box) const {
        const auto collision = GetRayCollisionBox(_ray, box.boundingBox());
        return {.hit = collision.hit, .distance = collision.distance};
    }

  private:
    ::Ray _ray{};
};
}  // namespace zappy::gui::raylib::rcore
