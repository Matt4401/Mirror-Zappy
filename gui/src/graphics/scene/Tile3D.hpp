/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Tile3D
*/

#pragma once

#include "game/ItemBag.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::graphics::scene {
class Tile3D {
  public:
    static constexpr float TILE_SIZE = 4.0F;

    explicit Tile3D(raylib::rmath::Vector3 position = {0.0F, 0.0F, 0.0F});
    ~Tile3D() = default;
    Tile3D(const Tile3D& other) = delete;
    Tile3D& operator=(const Tile3D& other) = delete;
    Tile3D(Tile3D&& other) noexcept = default;
    Tile3D& operator=(Tile3D&& other) noexcept = default;

    void draw(const raylib::rmodels::Model& model) const;

    [[nodiscard]] raylib::rmath::Vector3 position() const { return _position; }
    [[nodiscard]] const game::ItemBag& itemBag() const { return _itemBag; }
    [[nodiscard]] game::ItemBag& itemBag() { return _itemBag; }

  protected:
  private:
    raylib::rmath::Vector3 _position;
    game::ItemBag _itemBag;
};
}  // namespace zappy::gui::graphics::scene
