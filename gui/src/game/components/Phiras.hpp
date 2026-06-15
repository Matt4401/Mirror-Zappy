/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Phiras
*/

#pragma once
#include <string>

#include "AObject.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"
#include "Color.hpp"
#include "graphics/scene/Tile3D.hpp"

namespace zappy::gui::game {
class Phiras : public AObject {
  public:
    Phiras(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Phiras");
        setDensity(0.08);
        setPositionY(position.y() + graphics::scene::Tile3D::TILE_SIZE);
        setPositionX(position.x() - 6.4F);
        setPositionZ(position.z() + 0.4F);
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
