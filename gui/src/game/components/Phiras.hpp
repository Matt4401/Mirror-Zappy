/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Phiras
*/

#pragma once

#include "AObject.hpp"
#include "Color.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::game {
class Phiras : public AObject {
  public:
    static constexpr auto PHIRAS_DENSITY = 0.08F;
    static constexpr auto PHIRAS_X_OFFSET = 6.4F;
    static constexpr auto PHIRAS_Z_OFFSET = 0.4F;

    Phiras(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Phiras");
        setDensity(PHIRAS_DENSITY);
        setPositionY(position.y() + graphics::scene::Tile3D::TILE_SIZE);
        setPositionX(position.x() - PHIRAS_X_OFFSET);
        setPositionZ(position.z() + PHIRAS_Z_OFFSET);
    }
    ~Phiras() override = default;
    Phiras(const Phiras& other) = delete;
    Phiras& operator=(const Phiras& other) = delete;
    Phiras(Phiras&& other) noexcept = default;
    Phiras& operator=(Phiras&& other) noexcept = default;

    void draw(const raylib::rmodels::Model& model) const override {
        model.drawModel(position(), 0.5F, raylib::Color::White());
    }

  protected:
  private:
};
}  // namespace zappy::gui::game
