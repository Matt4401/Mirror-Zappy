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

    Player(int id, raylib::rmath::Vector3 position, std::string name, cardinalPoint orientation)
        : _id(id), _position(position), _name(std::move(name)), _orientation(orientation) {}
    ~Player() = default;
    Player(const Player& other) = delete;
    Player& operator=(const Player& other) = delete;
    Player(Player&& other) noexcept = default;
    Player& operator=(Player&& other) noexcept = default;

    [[nodiscard]] int id() const { return _id; }
    [[nodiscard]] raylib::rmath::Vector3 position() const { return {_position.x(), _position.y(), _position.z()}; }
    [[nodiscard]] const std::string& name() const { return _name; }
    [[nodiscard]] cardinalPoint orientation() const { return _orientation; }
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
};
}  // namespace zappy::gui::game
