/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Tile3D
*/

#include "Tile3D.hpp"
#include "Color.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::graphics::scene {
Tile3D::Tile3D(raylib::rmath::Vector3 position) : _position{position} {}

void Tile3D::draw(const raylib::rmodels::Model& model) const {
    model.drawModel(_position, 2.0F, raylib::Color::White());
}

}  // namespace zappy::gui::graphics::scene
