/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** BoundingBox
*/

#pragma once

#include <raylib.h>

#include "rmath/Vector3.hpp"

namespace zappy::gui::raylib::rcore {
class BoundingBox {
  public:
    BoundingBox(rmath::Vector3 min, rmath::Vector3 max) : _boundingBox{.min = min.vector(), .max = max.vector()} {}
    explicit BoundingBox(::BoundingBox bb) : _boundingBox{bb} {}

    [[nodiscard]] rmath::Vector3 min() const { return _boundingBox.min; }
    [[nodiscard]] rmath::Vector3 max() const { return _boundingBox.max; }
    [[nodiscard]] ::BoundingBox boundingBox() const { return _boundingBox; }

  private:
    ::BoundingBox _boundingBox{};
};
}  // namespace zappy::gui::raylib::rcore
