/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Player
*/

#include "Player.hpp"

#include <cmath>
#include <cstddef>
#include <string>
#include <utility>

#include "graphics/scene/Tile3D.hpp"
#include "rcore/BoundingBox.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::game {
float Player::angleForOrientation(const Player::cardinalPoint orientation) {
    switch (orientation) {
        case Player::cardinalPoint::NORTH:
            return NorthAngle;
        case Player::cardinalPoint::EAST:
            return EastAngle;
        case Player::cardinalPoint::SOUTH:
            return SouthAngle;
        case Player::cardinalPoint::WEST:
            return WestAngle;
    }
    return SouthAngle;
}

float Player::normalizeAngle(float angle) {
    angle = std::fmod(angle, FullTurnDegrees);
    if (angle < 0.0F) {
        angle += FullTurnDegrees;
    }
    return angle;
}

float Player::shortestAngleDelta(const float from, const float to) {
    float delta = normalizeAngle(to) - normalizeAngle(from);
    if (delta > HalfTurnDegrees) {
        delta -= FullTurnDegrees;
    } else if (delta < -HalfTurnDegrees) {
        delta += FullTurnDegrees;
    }
    return delta;
}

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
      _renderRotationAngle(angleForOrientation(orientation)),
      _targetRotationAngle(angleForOrientation(orientation)),
      _level(level) {}

void Player::setTilePosition(const graphics::scene::Tile3DPosition tilePosition) { _tilePosition = tilePosition; }

void Player::move(const int serverFrequency, const float deltaTime) {
    if ((!_isMoving && !_isTurning && _offset == _targetOffset) || serverFrequency <= 0 || deltaTime <= 0.0F) {
        return;
    }

    const float serverSpeed = static_cast<float>(serverFrequency) / DELTA_SERVER_FREQUENCY;
    const float movement = PLAYER_SPEED * serverSpeed * deltaTime;

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

    if (_isTurning) {
        const float rotationMovement = (movement / graphics::scene::Tile3D::TILE_SIZE) * DegreesPerQuarterTurn;
        const float delta = shortestAngleDelta(_renderRotationAngle, _targetRotationAngle);
        if (std::fabs(delta) <= rotationMovement) {
            _renderRotationAngle = _targetRotationAngle;
            _isTurning = false;
        } else {
            _renderRotationAngle =
                normalizeAngle(_renderRotationAngle + (delta > 0.0F ? rotationMovement : -rotationMovement));
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

void Player::setOrientation(const cardinalPoint orientation) {
    _orientation = orientation;
    _renderRotationAngle = angleForOrientation(orientation);
    _targetRotationAngle = _renderRotationAngle;
    _isTurning = false;
}

void Player::turnToOrientation(const cardinalPoint orientation) {
    _orientation = orientation;
    _targetRotationAngle = angleForOrientation(orientation);
    _isTurning = shortestAngleDelta(_renderRotationAngle, _targetRotationAngle) != 0.0F;
}

raylib::rmath::Vector3 Player::renderDirection() const {
    const float radians = _renderRotationAngle * DegreesToRadians;
    return {std::sin(radians), 0.0F, std::cos(radians)};
}
}  // namespace zappy::gui::game
