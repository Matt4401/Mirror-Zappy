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
Tile3D::Tile3D(int gridX, int gridY, raylib::rmath::Vector3 position)
    : _gridX{gridX}, _gridY{gridY}, _position{position} {}

void Tile3D::draw(const raylib::rmodels::Model& model) const {
    model.drawModel(_position, TILE_SCALE, raylib::Color::White());
}

raylib::rcore::BoundingBox Tile3D::boundingBox(const raylib::rcore::BoundingBox& modelBB) const {
    raylib::rmath::Vector3 const min{_position.x() + (modelBB.min().x() * TILE_SCALE),
                                     _position.y() + (modelBB.min().y() * TILE_SCALE),
                                     _position.z() + (modelBB.min().z() * TILE_SCALE)};
    raylib::rmath::Vector3 const max{_position.x() + (modelBB.max().x() * TILE_SCALE),
                                     _position.y() + (modelBB.max().y() * TILE_SCALE),
                                     _position.z() + (modelBB.max().z() * TILE_SCALE)};
    return {min, max};
}

}  // namespace zappy::gui::graphics::scene
