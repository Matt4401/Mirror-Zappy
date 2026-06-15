/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Thystame
*/

#pragma once
#include <string>
#include "Tile3D.hpp"
#include "AObject.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"
#include "Color.hpp"

namespace zappy::gui::game {
class Thystame : public AObject {
  public:
    Thystame(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Thystame");
        setDensity(0.05);
        setPositionY(position.y() + graphics::scene::Tile3D::TILE_SIZE);
        setPositionX(position.x() - 7.0F);
        setPositionZ(position.z() - 0.5F);
    }
    ~Thystame() override = default;
    Thystame(const Thystame& other) = delete;
    Thystame& operator=(const Thystame& other) = delete;
    Thystame(Thystame&& other) noexcept = default;
    Thystame& operator=(Thystame&& other) noexcept = default;
    void draw(const raylib::rmodels::Model& model) const override {
        model.drawModel(position(), 0.5F, raylib::Color::White());
    }

  protected:
  private:
};
}  // namespace zappy::gui::game
