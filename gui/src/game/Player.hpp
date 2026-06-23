/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Player
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>

#include "ItemBag.hpp"
#include "rcore/BoundingBox.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::game {
class Player {
  public:
    enum class cardinalPoint : std::uint8_t { NORTH = 1, EAST = 2, SOUTH = 3, WEST = 4 };

    Player(int id, raylib::rmath::Vector3 position, std::string name, cardinalPoint orientation, std::size_t level = 1)
        : _id(id), _position(position), _name(std::move(name)), _orientation(orientation), _level(level) {}
    ~Player() = default;
    Player(const Player& other) = delete;
    Player& operator=(const Player& other) = delete;
    Player(Player&& other) noexcept = default;
    Player& operator=(Player&& other) noexcept = default;

    [[nodiscard]] int id() const { return _id; }
    [[nodiscard]] raylib::rmath::Vector3 position() const { return {_position.x(), _position.y(), _position.z()}; }
    [[nodiscard]] const std::string& name() const { return _name; }
    void setName(const std::string& name) { _name = name; }
    [[nodiscard]] cardinalPoint orientation() const { return _orientation; }
    [[nodiscard]] std::size_t level() const { return _level; }
    [[nodiscard]] int tileX() const { return _tileX; }
    [[nodiscard]] int tileY() const { return _tileY; }
    [[nodiscard]] const game::ItemBag& itemBag() const { return _itemBag; }
    [[nodiscard]] game::ItemBag& itemBag() { return _itemBag; }
    void setPosition(const raylib::rmath::Vector3& position) { _position = position; }
    void setTilePosition(int x, int y) {
        _tileX = x;
        _tileY = y;
    }
    void setOrientation(cardinalPoint orientation) { _orientation = orientation; }
    void setLevel(std::size_t level) { _level = level; }
    void setTextureId(const std::string& textureId) { _textureId = textureId; }
    [[nodiscard]] std::string textureId() const { return _textureId; }
    [[nodiscard]] raylib::rcore::BoundingBox boundingBox() const;

  protected:
  private:
    int _id{0};
    game::ItemBag _itemBag;
    raylib::rmath::Vector3 _position{10.0F, 10.0F, 0.0F};
    std::string _name;
    cardinalPoint _orientation{cardinalPoint::NORTH};
    std::size_t _level{1};
    std::size_t _life{10};
    std::string _textureId;
    int _tileX{0};
    int _tileY{0};
};
}  // namespace zappy::gui::game
