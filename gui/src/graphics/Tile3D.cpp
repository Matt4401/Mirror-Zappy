/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Tile3D
*/

#include "Tile3D.hpp"

#include "Color.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::graphics {

Tile3D::Tile3D() : _model{MODEL_PATH} {}

void Tile3D::draw(raylib::rmath::Vector3 position) const {
    _model.drawModel(position, 1.0F, raylib::Color::White().color());
}

}  // namespace zappy::gui::graphics
