/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Player
*/

#include "Player.hpp"

#include <algorithm>
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

    const auto pos = position();
    return {
        {pos.x() - PICK_RADIUS, pos.y() - PICK_RADIUS, pos.z() - PICK_RADIUS},
        {pos.x() + PICK_RADIUS, pos.y() + PICK_HEIGHT, pos.z() + PICK_RADIUS},
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
    if ((!_isMoving && _offset == _targetOffset) || serverFrequency <= 0 || deltaTime <= 0.0F) {
        _animFrame = 0;
        return;
    }

    float const distanceToTarget = _position.distance(_futurePosition);
    float catchUpMultiplier = 1.0F;
    if (distanceToTarget > 2.0F) {
        catchUpMultiplier = distanceToTarget / 2.0F;
    }

    const float movement = PLAYER_SPEED * catchUpMultiplier * deltaTime;

    _animFrame += std::max(2, static_cast<int>(movement * 30.0F));

    if (_isMoving) {
        if (_position.distance(_futurePosition) <= movement) {
            _position = _futurePosition;
            if (_wrappedPosition.has_value()) {
                _position = _wrappedPosition.value();
                _futurePosition = _position;
                _wrappedPosition.reset();
            }
            _isMoving = false;
        } else {
            _position = _position.movedTowards(_futurePosition, movement);
        }
    }

    if (_offset != _targetOffset) {
        if (_offset.distance(_targetOffset) <= movement) {
            _offset = _targetOffset;
        } else {
            _offset = _offset.movedTowards(_targetOffset, movement);
        }
    }
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
