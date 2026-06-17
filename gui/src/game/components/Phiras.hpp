/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Phiras
*/

#pragma once

#include "AObject.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::game {
class Phiras : public AObject {
  public:
    static constexpr auto PHIRAS_SCALE = 0.5F;
    static constexpr auto PHIRAS_X_OFFSET = 6.4F;
    static constexpr auto PHIRAS_Z_OFFSET = 0.4F;

    Phiras(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Phiras");
        setScale(PHIRAS_SCALE);
        setPositionY(position.y() + graphics::scene::Tile3D::TILE_SIZE);
        setPositionX(position.x() - PHIRAS_X_OFFSET);
        setPositionZ(position.z() + PHIRAS_Z_OFFSET);
    }
    ~Phiras() override = default;
    Phiras(const Phiras& other) = delete;
    Phiras& operator=(const Phiras& other) = delete;
    Phiras(Phiras&& other) noexcept = default;
    Phiras& operator=(Phiras&& other) noexcept = default;

  protected:
  private:
};
}  // namespace zappy::gui::game
