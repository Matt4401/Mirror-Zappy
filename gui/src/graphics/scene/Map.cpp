/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Map
*/

#include "Map.hpp"
#include "Tile3D.hpp"
#include "rcore/Camera.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::graphics::scene {
    Map::Map(int width, int height) {
        raylib::rmath::Vector3 position{0.0F, 0.0F, 0.0F};

        for (int x = ((width / 2) * -1); x < width / 2; x += 2) {
            for (int z = ((height / 2) * -1); z < height / 2; z += 2) {
                position.setX(static_cast<float>(x));
                position.setZ(static_cast<float>(z));
                _tiles.emplace_back(position);
            }
        }
    }

    void Map::draw(const raylib::rcore::Camera &camera) const {
        for (const auto& tile : _tiles) {
            if (camera.isVisibleFromCamera(tile.position())) {
                tile.draw(_tileModel);
            }
        }
    }
}