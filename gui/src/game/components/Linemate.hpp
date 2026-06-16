/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Linemate
*/

#pragma once

#include "AObject.hpp"
#include "Color.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::game {
class Linemate : public AObject {
  public:
    static constexpr auto LINEMATE_X_OFFSET = 0.86F;
    static constexpr auto LINEMATE_Z_OFFSET = 0.5F;
    static constexpr auto LINEMATE_DENSITY = 0.3F;

    Linemate(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Linemate");
        setDensity(LINEMATE_DENSITY);
        setPositionY(position.y() + graphics::scene::Tile3D::TILE_SIZE);
        setPositionX(position.x() - LINEMATE_X_OFFSET);
        setPositionZ(position.z() + LINEMATE_Z_OFFSET);
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
