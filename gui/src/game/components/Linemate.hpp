/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Linemate
*/

#pragma once
#include <string>

#include "AObject.hpp"
#include "Color.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::game {
class Linemate : public AObject {
  public:
    Linemate(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Linemate");
        setDensity(0.3);
        setPositionY(position.y() + graphics::scene::Tile3D::TILE_SIZE);
        setPositionX(position.x() - 0.86F);
        setPositionZ(position.z() + 0.5F);
    }
    ~Linemate() override = default;
    Linemate(const Linemate& other) = delete;
    Linemate& operator=(const Linemate& other) = delete;
    Linemate(Linemate&& other) noexcept = default;
    Linemate& operator=(Linemate&& other) noexcept = default;

    void draw(const raylib::rmodels::Model& model) const override {
        model.drawModel(position(), 0.5F, raylib::Color::White());
    }

  protected:
  private:
};
}  // namespace zappy::gui::game
