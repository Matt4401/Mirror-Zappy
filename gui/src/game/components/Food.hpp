/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Food
*/

#pragma once
#include <string>

#include "AObject.hpp"
#include "Color.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::game {
class Food : public AObject {
  public:
    Food(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Food");
        setDensity(0.5);
        setPositionY(position.y() + (graphics::scene::Tile3D::TILE_SIZE * 1.03F));
        setPositionZ(position.z() - 2.2F);
    }
    ~Food() override = default;
    Food(const Food& other) = delete;
    Food& operator=(const Food& other) = delete;
    Food(Food&& other) noexcept = default;
    Food& operator=(Food&& other) noexcept = default;

    void draw(const raylib::rmodels::Model& model) const override {
        model.drawModel(position(), 0.9F, raylib::Color::Yellow());
    }

  protected:
  private:
};
}  // namespace zappy::gui::game
