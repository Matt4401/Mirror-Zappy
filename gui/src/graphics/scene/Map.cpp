/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Map
*/

#include "Map.hpp"

#include <memory>

#include "Tile3D.hpp"
#include "components/Deraumere.hpp"
#include "components/IObject.hpp"
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

            // TEMPORARY : Add a Deraumere item to the tile for testing
            raylib::rmath::Vector3 itemPosition = position;
            itemPosition.setY(position.y() + TILE_SIZE);
            _tiles.back().addItem(std::make_unique<Deraumere>(itemPosition));
        }
    }

    _itemDrawFunctions["Deraumere"] = [this](const IObject& object) { drawDeraumere(object); };
}

void Map::draw(const raylib::rcore::Camera& camera) const {
    for (const auto& tile : _tiles) {
        if (camera.isVisibleFromCamera(tile.position())) {
            tile.draw(_tileModel);
            if (tile.hasItems()) {
                drawItems(tile);
            }
        }
    }
}

void Map::drawItems(const Tile3D& tile) const {
    for (const auto& item : tile.items()) {
        auto it = _itemDrawFunctions.find(item.object->name());
        if (it != _itemDrawFunctions.end()) {
            it->second(*item.object);
        }
    }
}

void Map::drawDeraumere(const IObject& object) const {
    const auto* deraumereObject = dynamic_cast<const Deraumere*>(&object);
    if (deraumereObject == nullptr) {
        return;
    }
    deraumereObject->draw(_deraumereModel);
}
}  // namespace zappy::gui::graphics::scene
