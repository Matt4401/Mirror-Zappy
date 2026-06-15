/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Map
*/

#include "Map.hpp"

#include <memory>

#include "Tile3D.hpp"
#include "game/components/IObject.hpp"
#include "rcore/Camera.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::graphics::scene {
Map::Map(int width, int height) {
    resize(width, height);
    _itemDrawFunctions["Deraumere"] = [this](const game::IObject& object) { object.draw(_deraumereModel); };
    _itemDrawFunctions["Linemate"] = [this](const game::IObject& object) { object.draw(_linemateModel); };
    _itemDrawFunctions["Sibur"] = [this](const game::IObject& object) { object.draw(_siburModel); };
    _itemDrawFunctions["Phiras"] = [this](const game::IObject& object) { object.draw(_phirasModel); };
    _itemDrawFunctions["Thystame"] = [this](const game::IObject& object) { object.draw(_thystameModel); };
    _itemDrawFunctions["Mendiane"] = [this](const game::IObject& object) { object.draw(_mendianeModel); };
    _itemDrawFunctions["Food"] = [this](const game::IObject& object) { object.draw(_foodModel); };
}

void Map::resize(int width, int height) {
    _tiles.clear();
    raylib::rmath::Vector3 position{0.0F, 0.0F, 0.0F};

    for (int x = ((width / 2) * -1); x < width / 2; x += 1) {
        for (int z = ((height / 2) * -1); z < height / 2; z += 1) {
            position.setX(static_cast<float>(static_cast<float>(x) * Tile3D::TILE_SIZE));
            position.setZ(static_cast<float>(static_cast<float>(z) * Tile3D::TILE_SIZE));
            _tiles.emplace_back(position);
        }
        // TEMPORARY : Add a Deraumere item to the tile for testing
        //  _tiles.back().itemBag().addItem(std::make_unique<game::Deraumere>(position));
        // _tiles.back().itemBag().addItem(std::make_unique<game::Linemate>(position));
        // _tiles.back().itemBag().addItem(std::make_unique<game::Sibur>(position));
        // _tiles.back().itemBag().addItem(std::make_unique<game::Phiras>(position));
        // _tiles.back().itemBag().addItem(std::make_unique<game::Thystame>(position));
        // _tiles.back().itemBag().addItem(std::make_unique<game::Mendiane>(position));
        // _tiles.back().itemBag().addItem(std::make_unique<game::Food>(position));
    }
}

void Map::draw(const raylib::rcore::Camera& camera) const {
    for (const auto& tile : _tiles) {
        if (camera.isVisibleFromCamera(tile.position())) {
            tile.draw(_tileModel);
            if (tile.itemBag().hasItems()) {
                drawItems(tile);
            }
        }
    }
}

void Map::drawItems(const Tile3D& tile) const {
    for (const auto& item : tile.itemBag().items()) {
        auto it = _itemDrawFunctions.find(item.object->name());
        if (it != _itemDrawFunctions.end()) {
            it->second(*item.object);
        }
    }
}
}  // namespace zappy::gui::graphics::scene
