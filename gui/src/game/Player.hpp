/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Player
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

#include "ItemBag.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "rcore/BoundingBox.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::game {
class Player {
  public:
    enum class cardinalPoint : std::uint8_t { NORTH = 1, EAST = 2, SOUTH = 3, WEST = 4 };
    static constexpr float PLAYER_SPEED = 20.0F;
    static constexpr int DELTA_SERVER_FREQUENCY = 20;
    static constexpr float DegreesPerQuarterTurn = 90.0F;
    static constexpr float FullTurnDegrees = 360.0F;
    static constexpr float HalfTurnDegrees = 180.0F;
    static constexpr float SouthAngle = 180.0F;
    static constexpr float EastAngle = 270.0F;
    static constexpr float NorthAngle = 0.0F;
    static constexpr float WestAngle = 90.0F;

    Player(int id, raylib::rmath::Vector3 position, std::string name, cardinalPoint orientation, std::size_t level = 1);
    ~Player() = default;
    Player(const Player& other) = delete;
    Player& operator=(const Player& other) = delete;
    Player(Player&& other) noexcept = default;
    Player& operator=(Player&& other) noexcept = default;

    [[nodiscard]] int id() const { return _id; }
    [[nodiscard]] raylib::rmath::Vector3 position() const {
        return {_position.x() + _offset.x(), _position.y() + _offset.y(), _position.z() + _offset.z()};
    }
    [[nodiscard]] raylib::rmath::Vector3 basePosition() const { return _position; }
    [[nodiscard]] const std::string& name() const { return _name; }
    void setName(const std::string& name) { _name = name; }
    [[nodiscard]] cardinalPoint orientation() const { return _orientation; }
    [[nodiscard]] float renderRotationAngle() const { return _renderRotationAngle; }
    [[nodiscard]] raylib::rmath::Vector3 renderDirection() const;
    [[nodiscard]] std::size_t level() const { return _level; }
    [[nodiscard]] graphics::scene::Tile3DPosition tilePosition() const { return _tilePosition; }
    [[nodiscard]] const game::ItemBag& itemBag() const { return _itemBag; }
    [[nodiscard]] game::ItemBag& itemBag() { return _itemBag; }
    void setTargetOffset(const raylib::rmath::Vector3& offset) { _targetOffset = offset; }
    void snapOffset(const raylib::rmath::Vector3& offset) {
        _offset = offset;
        _targetOffset = offset;
    }
    void setPosition(const raylib::rmath::Vector3& position);
    void setFuturePosition(const raylib::rmath::Vector3& position);
    void setWrappedFuturePosition(const raylib::rmath::Vector3& exitPosition,
                                  const raylib::rmath::Vector3& wrappedPosition);
    [[nodiscard]] const raylib::rmath::Vector3& futurePosition() const { return _futurePosition; }
    void setTilePosition(graphics::scene::Tile3DPosition tilePosition);
    void setOrientation(cardinalPoint orientation);
    void turnToOrientation(cardinalPoint orientation);
    void setLevel(std::size_t level) { _level = level; }
    void setTextureId(const std::string& textureId) { _textureId = textureId; }
    [[nodiscard]] std::string textureId() const { return _textureId; }
    [[nodiscard]] raylib::rcore::BoundingBox boundingBox() const;
    [[nodiscard]] bool moving() const { return _isMoving; }
    void move(int serverFrequency, float deltaTime);

    static float angleForOrientation(cardinalPoint orientation);
    static float normalizeAngle(float angle);
    static float shortestAngleDelta(float from, float to);

  protected:
  private:
    bool _isMoving{false};
    bool _isTurning{false};
    int _id{0};
    game::ItemBag _itemBag;
    raylib::rmath::Vector3 _position{10.0F, 10.0F, 0.0F};
    raylib::rmath::Vector3 _futurePosition{10.0F, 10.0F, 0.0F};
    raylib::rmath::Vector3 _offset{0.0F, 0.0F, 0.0F};
    raylib::rmath::Vector3 _targetOffset{0.0F, 0.0F, 0.0F};
    std::optional<raylib::rmath::Vector3> _wrappedPosition;
    std::string _name;
    cardinalPoint _orientation{cardinalPoint::NORTH};
    float _renderRotationAngle{0.0F};
    float _targetRotationAngle{0.0F};
    std::size_t _level{1};
    std::size_t _life{10};
    std::string _textureId;
    graphics::scene::Tile3DPosition _tilePosition;
};
}  // namespace zappy::gui::game
