/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Tile3D
*/

#pragma once
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::graphics {
class Tile3D {
  public:
    static constexpr const char* MODEL_PATH = "assets/minecraft-grass-block/source/Grass_Block.obj";

    Tile3D();
    ~Tile3D() = default;
    Tile3D(const Tile3D& other) = delete;
    Tile3D& operator=(const Tile3D& other) = delete;
    Tile3D(Tile3D&& other) noexcept = default;
    Tile3D& operator=(Tile3D&& other) noexcept = default;

    void draw(raylib::rmath::Vector3 position) const;

  protected:
  private:
    raylib::rmodels::Model _model;
};
}  // namespace zappy::gui::graphics
