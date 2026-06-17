/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Sibur
*/

#pragma once

#include "AObject.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::game {
class Sibur : public AObject {
  public:
    static constexpr auto SIBUR_SCALE = 0.5F;
    static constexpr auto SIBUR_X_OFFSET = 1.2F;
    static constexpr auto SIBUR_Z_OFFSET = 2.0F;

    Sibur(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Sibur");
        setScale(SIBUR_SCALE);
        setPositionY(position.y() + graphics::scene::Tile3D::TILE_SIZE);
        setPositionX(position.x() - SIBUR_X_OFFSET);
        setPositionZ(position.z() - SIBUR_Z_OFFSET);
    }
    ~Sibur() override = default;
    Sibur(const Sibur& other) = delete;
    Sibur& operator=(const Sibur& other) = delete;
    Sibur(Sibur&& other) noexcept = default;
    Sibur& operator=(Sibur&& other) noexcept = default;

  protected:
  private:
};
}  // namespace zappy::gui::game
