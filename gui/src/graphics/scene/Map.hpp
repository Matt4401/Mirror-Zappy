/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Map
*/

#pragma once
#include <vector>
#include "Tile3D.hpp"
#include "rmodels/Model.hpp"
#include "rcore/Camera.hpp"

namespace zappy::gui::graphics::scene {
class Map {
public:
    static constexpr const char* TILE_MODEL_RESOURCE = "assets/minecraft-grass-block/source/Grass_Block.obj";

    Map(int width, int height);
    ~Map() = default;
    Map(const Map& other) = delete;
    Map& operator=(const Map& other) = delete;
    Map(Map&& other) noexcept = default;
    Map& operator=(Map&& other) noexcept = default;

    void draw(const raylib::rcore::Camera &camera) const;

protected:
private:
    std::vector<Tile3D> _tiles;
    raylib::rmodels::Model _tileModel{TILE_MODEL_RESOURCE};
};
} // namespace zappy::gui::graphics
