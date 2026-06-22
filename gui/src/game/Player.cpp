/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Player
*/

#include "Player.hpp"

#include "rcore/BoundingBox.hpp"

namespace zappy::gui::game {
raylib::rcore::BoundingBox Player::boundingBox() const {
    static constexpr float PICK_RADIUS = 0.8F;
    static constexpr float PICK_HEIGHT = 2.4F;

    return {
        {_position.x() - PICK_RADIUS, _position.y() - PICK_RADIUS, _position.z() - PICK_RADIUS},
        {_position.x() + PICK_RADIUS, _position.y() + PICK_HEIGHT, _position.z() + PICK_RADIUS},
    };
}
}  // namespace zappy::gui::game
