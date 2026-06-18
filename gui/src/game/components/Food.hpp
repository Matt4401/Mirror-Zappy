/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Food
*/

#pragma once

#include "AObject.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::game {
class Food : public AObject {
  public:
    static constexpr auto FOOD_Y_OFFSET = 1.03F;
    static constexpr auto FOOD_Z_OFFSET = 2.2F;
    static constexpr auto FOOD_SCALE = 0.9F;

    Food(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Food");
        setScale(FOOD_SCALE);
        setPositionY(position.y() + (graphics::scene::Tile3D::TILE_SIZE * FOOD_Y_OFFSET));
        setPositionZ(position.z() - FOOD_Z_OFFSET);
    }
    ~Food() override = default;
    Food(const Food& other) = delete;
    Food& operator=(const Food& other) = delete;
    Food(Food&& other) noexcept = default;
    Food& operator=(Food&& other) noexcept = default;

  protected:
  private:
};
}  // namespace zappy::gui::game
