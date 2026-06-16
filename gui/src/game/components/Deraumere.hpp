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
    static constexpr auto DERAUMERE_X_OFFSET = 0.4F;
    static constexpr auto DERAUMERE_Z_OFFSET = 0.5F;
    static constexpr auto DERAUMERE_DENSITY = 0.15F;

    Deraumere(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Deraumere");
        setDensity(DERAUMERE_DENSITY);
        setPositionY(position.y() + graphics::scene::Tile3D::TILE_SIZE);
        setPositionX(position.x() - DERAUMERE_X_OFFSET);
        setPositionZ(position.z() - DERAUMERE_Z_OFFSET);
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
