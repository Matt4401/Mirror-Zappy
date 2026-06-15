/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Deraumere
*/

#pragma once

#include "AObject.hpp"
#include "Color.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::game {
class Deraumere : public AObject {
  public:
    Deraumere(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Deraumere");
        setDensity(0.15);
        setPositionY(position.y() + graphics::scene::Tile3D::TILE_SIZE);
        setPositionX(position.x() - 0.4F);
        setPositionZ(position.z() - 0.5F);
    }
    ~Deraumere() override = default;
    Deraumere(const Deraumere& other) = delete;
    Deraumere& operator=(const Deraumere& other) = delete;
    Deraumere(Deraumere&& other) noexcept = default;
    Deraumere& operator=(Deraumere&& other) noexcept = default;

    void draw(const raylib::rmodels::Model& model) const override {
        model.drawModel(position(), 0.5F, raylib::Color::White());
    }

  protected:
  private:
};
}  // namespace zappy::gui::game
