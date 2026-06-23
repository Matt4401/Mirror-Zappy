/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Egg
*/

#pragma once

#include "rmath/Vector3.hpp"

namespace zappy::gui::game {
class Egg {
  public:
    Egg() = default;
    Egg(int id, int playerId, raylib::rmath::Vector3 position) : _id(id), _playerId(playerId), _position(position) {}
    ~Egg() = default;
    Egg(const Egg& other) = delete;
    Egg& operator=(const Egg& other) = delete;
    Egg(Egg&& other) noexcept = default;
    Egg& operator=(Egg&& other) noexcept = default;

    [[nodiscard]] int id() const { return _id; }
    [[nodiscard]] int playerId() const { return _playerId; }
    [[nodiscard]] raylib::rmath::Vector3 position() const { return {_position.x(), _position.y(), _position.z()}; }

  protected:
  private:
    int _id{-1};
    int _playerId{-1};
    raylib::rmath::Vector3 _position{0.0F, 0.0F, 0.0F};
};
}  // namespace zappy::gui::game
