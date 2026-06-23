/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Player
*/

#include "Player.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "rcore/BoundingBox.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::game {
raylib::rcore::BoundingBox Player::boundingBox() const {
    static constexpr float PICK_RADIUS = 0.8F;
    static constexpr float PICK_HEIGHT = 2.4F;

    return {
        {_position.x() - PICK_RADIUS, _position.y() - PICK_RADIUS, _position.z() - PICK_RADIUS},
        {_position.x() + PICK_RADIUS, _position.y() + PICK_HEIGHT, _position.z() + PICK_RADIUS},
    };
}

Player::Player(int id, raylib::rmath::Vector3 position, std::string name, cardinalPoint orientation, std::size_t level)
    : _id(id), _position(position), _name(std::move(name)), _orientation(orientation), _level(level) {}

void Player::setTilePosition(const graphics::scene::Tile3DPosition tilePosition) { _tilePosition = tilePosition; }
}  // namespace zappy::gui::game
