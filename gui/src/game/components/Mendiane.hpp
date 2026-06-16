/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Mendiane
*/

#pragma once

#include "AObject.hpp"
#include "Color.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::game {
class Mendiane : public AObject {
  public:
    static constexpr auto MENDIANE_DENSITY = 0.1F;
    static constexpr auto MENDIANE_X_OFFSET = 1.3F;
    static constexpr auto MENDIANE_Z_OFFSET = 1.2F;

    Mendiane(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Mendiane");
        setDensity(MENDIANE_DENSITY);
        setPositionY(position.y() + graphics::scene::Tile3D::TILE_SIZE);
        setPositionX(position.x() - MENDIANE_X_OFFSET);
        setPositionZ(position.z() - MENDIANE_Z_OFFSET);
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
