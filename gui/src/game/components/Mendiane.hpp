/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Mendiane
*/

#pragma once
#include <string>

#include "AObject.hpp"
#include "Color.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::game {
class Mendiane : public AObject {
  public:
    Mendiane(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Mendiane");
        setDensity(0.1);
        setPositionY(position.y() + graphics::scene::Tile3D::TILE_SIZE);
        setPositionX(position.x() - 1.3F);
        setPositionZ(position.z() - 1.2F);
    }
    ~Mendiane() override = default;
    Mendiane(const Mendiane& other) = delete;
    Mendiane& operator=(const Mendiane& other) = delete;
    Mendiane(Mendiane&& other) noexcept = default;
    Mendiane& operator=(Mendiane&& other) noexcept = default;

    void draw(const raylib::rmodels::Model& model) const override {
        model.drawModel(position(), 0.5F, raylib::Color::White());
    }

  protected:
  private:
};
}  // namespace zappy::gui::game
