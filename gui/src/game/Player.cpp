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

#include "graphics/scene/Tile3D.hpp"
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
    : _id(id),
      _position(position),
      _futurePosition(position),
      _name(std::move(name)),
      _orientation(orientation),
      _level(level) {}

void Player::setTilePosition(const graphics::scene::Tile3DPosition tilePosition) { _tilePosition = tilePosition; }

void Player::move(const int serverFrequency, const float deltaTime) {
    if (!_isMoving || serverFrequency <= 0 || deltaTime <= 0.0F) {
        return;
    }

    const float serverSpeed = static_cast<float>(serverFrequency) / DELTA_SERVER_FREQUENCY;
    const float movement = PLAYER_SPEED * serverSpeed * deltaTime;

    if (_position.distance(_futurePosition) <= movement) {
        _position = _futurePosition;
        if (_wrappedPosition.has_value()) {
            _position = _wrappedPosition.value();
            _futurePosition = _position;
            _wrappedPosition.reset();
        }
        _isMoving = false;
        return;
    }
    _position = _position.movedTowards(_futurePosition, movement);
}

void Player::setPosition(const raylib::rmath::Vector3& position) {
    _position = position;
    _futurePosition = position;
    _wrappedPosition.reset();
    _isMoving = false;
}

void Player::setFuturePosition(const raylib::rmath::Vector3& position) {
    _futurePosition = position;
    _wrappedPosition.reset();
    _isMoving = _position != _futurePosition;
}

void Player::setWrappedFuturePosition(const raylib::rmath::Vector3& exitPosition,
                                      const raylib::rmath::Vector3& wrappedPosition) {
    _futurePosition = exitPosition;
    _wrappedPosition = wrappedPosition;
    _isMoving = true;
}
}  // namespace zappy::gui::game
