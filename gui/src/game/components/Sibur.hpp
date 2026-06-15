/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Sibur
*/

#pragma once
#include <string>

#include "AObject.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"
#include "Color.hpp"
#include "graphics/scene/Tile3D.hpp"

namespace zappy::gui::game {
class Sibur : public AObject {
  public:
    Sibur(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Sibur");
        setDensity(0.1);
        setPositionY(position.y() + graphics::scene::Tile3D::TILE_SIZE);
        setPositionX(position.x() - 1.2F);
        setPositionZ(position.z() - 2.0F);
    }
    ~Sibur() override = default;
    Sibur(const Sibur& other) = delete;
    Sibur& operator=(const Sibur& other) = delete;
    Sibur(Sibur&& other) noexcept = default;
    Sibur& operator=(Sibur&& other) noexcept = default;

    void draw(const raylib::rmodels::Model& model) const override {
      model.drawModel(position(), 0.5F, raylib::Color::White());
    }

  protected:
  private:
};
}  // namespace zappy::gui::game
