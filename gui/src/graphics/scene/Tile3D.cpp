/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Tile3D
*/

#include "Tile3D.hpp"

#include "Color.hpp"
#include "rcore/BoundingBox.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::graphics::scene {
Tile3D::Tile3D(const Tile3DPosition gridPosition, raylib::rmath::Vector3 position)
    : _gridPosition{gridPosition}, _position{position} {}

void Tile3D::draw(const raylib::rmodels::Model& model) const {
    model.drawModel(_position, TILE_SCALE, raylib::Color::White());
}

raylib::rcore::BoundingBox Tile3D::boundingBox() const {
    raylib::rmath::Vector3 const min{_position.x() - (TILE_SIZE / 2.0F), _position.y() - 0.5F,
                                     _position.z() - (TILE_SIZE / 2.0F)};
    raylib::rmath::Vector3 const max{_position.x() + (TILE_SIZE / 2.0F), _position.y() + 0.5F,
                                     _position.z() + (TILE_SIZE / 2.0F)};
    return {min, max};
}

}  // namespace zappy::gui::graphics::scene
