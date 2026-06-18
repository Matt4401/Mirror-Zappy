/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Thystame
*/

#pragma once

#include "AObject.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::game {
class Thystame : public AObject {
  public:
    static constexpr auto THYSTAME_SCALE = 0.5F;
    static constexpr auto THYSTAME_X_OFFSET = 7.0F;
    static constexpr auto THYSTAME_Z_OFFSET = 0.5F;

    Thystame(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Thystame");
        setScale(THYSTAME_SCALE);
        setPositionY(position.y() + graphics::scene::Tile3D::TILE_SIZE);
        setPositionX(position.x() - THYSTAME_X_OFFSET);
        setPositionZ(position.z() - THYSTAME_Z_OFFSET);
    }
    ~Thystame() override = default;
    Thystame(const Thystame& other) = delete;
    Thystame& operator=(const Thystame& other) = delete;
    Thystame(Thystame&& other) noexcept = default;
    Thystame& operator=(Thystame&& other) noexcept = default;

  protected:
  private:
};
}  // namespace zappy::gui::game
