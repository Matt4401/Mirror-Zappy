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
#include "gui/raylib/Color.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::game {
class Player {
  public:
    enum class cardinalPoint : std::uint8_t { NORTH = 1, EAST = 2, SOUTH = 3, WEST = 4 };

    Player(raylib::rmath::Vector3 position, std::string name, cardinalPoint orientation, std::string teamName,
           raylib::Color teamColor)
        : _position(position),
          _name(std::move(name)),
          _orientation(orientation),
          _teamName(std::move(teamName)),
          _teamColor(teamColor) {}
    ~Player() = default;
    Player(const Player& other) = delete;
    Player& operator=(const Player& other) = delete;
    Player(Player&& other) noexcept = default;
    Player& operator=(Player&& other) noexcept = default;

    [[nodiscard]] raylib::rmath::Vector3 position() const { return {_position.x(), _position.y(), _position.z()}; }
    [[nodiscard]] std::string name() const { return _name; }
    [[nodiscard]] cardinalPoint orientation() const { return _orientation; }
    [[nodiscard]] std::string teamName() const { return _teamName; }
    [[nodiscard]] raylib::Color teamColor() const { return _teamColor; }

    void setTextureId(const std::string& textureId) { _textureId = textureId; }
    [[nodiscard]] std::string textureId() const { return _textureId; }

  protected:
  private:
    game::ItemBag _itemBag;
    raylib::rmath::Vector3 _position{10.0F, 10.0F, 0.0F};
    std::string _name;
    cardinalPoint _orientation{cardinalPoint::NORTH};
    std::string _teamName;
    raylib::Color _teamColor;
    std::size_t _level{1};
    std::size_t _life{10};
    std::string _textureId;
};
}  // namespace zappy::gui::game
