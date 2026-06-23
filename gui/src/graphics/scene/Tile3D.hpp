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
struct Tile3DPosition {
    int x{0};
    int y{0};

    bool operator==(const Tile3DPosition& other) const = default;
};

class Tile3D {
  public:
    static constexpr float TILE_SIZE = 4.0F;
    static constexpr float TILE_SCALE = 2.0F;

    explicit Tile3D(Tile3DPosition gridPosition = {}, raylib::rmath::Vector3 position = {0.0F, 0.0F, 0.0F});
    ~Tile3D() = default;
    Tile3D(const Tile3D& other) = delete;
    Tile3D& operator=(const Tile3D& other) = delete;
    Tile3D(Tile3D&& other) noexcept = default;
    Tile3D& operator=(Tile3D&& other) noexcept = default;

    void draw(const raylib::rmodels::Model& model) const;

    [[nodiscard]] raylib::rmath::Vector3 position() const { return _position; }
    [[nodiscard]] Tile3DPosition gridPosition() const { return _gridPosition; }
    [[nodiscard]] raylib::rcore::BoundingBox boundingBox() const;
    [[nodiscard]] const game::ItemBag& itemBag() const { return _itemBag; }
    [[nodiscard]] game::ItemBag& itemBag() { return _itemBag; }

  protected:
  private:
    Tile3DPosition _gridPosition;
    raylib::rmath::Vector3 _position;
    game::ItemBag _itemBag;
};
}  // namespace zappy::gui::graphics::scene
