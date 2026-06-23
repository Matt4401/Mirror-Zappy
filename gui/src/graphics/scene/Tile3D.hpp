/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Tile3D
*/

#pragma once

#include "game/ItemBag.hpp"
#include "rcore/BoundingBox.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::graphics::scene {
class Tile3D {
  public:
    static constexpr float TILE_SIZE = 4.0F;
    static constexpr float TILE_SCALE = 2.0F;

    explicit Tile3D(int gridX = 0, int gridY = 0, raylib::rmath::Vector3 position = {0.0F, 0.0F, 0.0F});
    ~Tile3D() = default;
    Tile3D(const Tile3D& other) = delete;
    Tile3D& operator=(const Tile3D& other) = delete;
    Tile3D(Tile3D&& other) noexcept = default;
    Tile3D& operator=(Tile3D&& other) noexcept = default;

    void draw(const raylib::rmodels::Model& model) const;

    [[nodiscard]] raylib::rmath::Vector3 position() const { return _position; }
    [[nodiscard]] int gridX() const { return _gridX; }
    [[nodiscard]] int gridY() const { return _gridY; }
    [[nodiscard]] raylib::rcore::BoundingBox boundingBox(const raylib::rcore::BoundingBox& modelBB) const;
    [[nodiscard]] const game::ItemBag& itemBag() const { return _itemBag; }
    [[nodiscard]] game::ItemBag& itemBag() { return _itemBag; }

  protected:
  private:
    int _gridX{0};
    int _gridY{0};
    raylib::rmath::Vector3 _position;
    game::ItemBag _itemBag;
};
}  // namespace zappy::gui::graphics::scene
