/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Map
*/

#include "Map.hpp"

#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <utility>

#include "Tile3D.hpp"
#include "events/EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "game/Player.hpp"
#include "game/Team.hpp"
#include "game/components/IObject.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::graphics::scene {
Map::Map(int width, int height, std::shared_ptr<raylib::rcore::Camera> camera,
         std::shared_ptr<events::EventDispatcher> dispatcher)
    : _camera(std::move(camera)), _dispatcher(std::move(dispatcher)), _gameModel(_camera) {
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
    _width = width;
    _height = height;
    _tiles.clear();
    raylib::rmath::Vector3 position{0.0F, 0.0F, 0.0F};

    for (int x = ((width / 2) * -1); x < width / 2; x += 1) {
        for (int z = ((height / 2) * -1); z < height / 2; z += 1) {
            position.setX(static_cast<float>(static_cast<float>(x) * Tile3D::TILE_SIZE));
            position.setZ(static_cast<float>(static_cast<float>(z) * Tile3D::TILE_SIZE));
            _tiles.emplace_back(position);
        }
    }
}

void Map::draw() const {
    for (const auto& tile : _tiles) {
        if (_camera->isVisibleFromCamera(tile.position())) {
            tile.draw(_tileModel);
            if (tile.itemBag().hasItems()) {
                drawItems(tile);
            }
        }
    }
    for (const auto& team : _teams) {
        team.draw(_gameModel);
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

void Map::handleEvent(const raylib::rcore::Event& event) {
    if (!raylib::rcore::Event::isMouseButtonPressed(MOUSE_LEFT_CLICK) || !_camera) {
        return;
    }

    const auto ray = event.mouseRay(*_camera);
    std::optional<SelectedPlayer> selected;
    float nearestDistance = std::numeric_limits<float>::max();

    for (const auto& team : _teams) {
        for (const auto& player : team.players()) {
            const auto collision = ray.collision(player.boundingBox());
            if (collision.hit && collision.distance < nearestDistance) {
                nearestDistance = collision.distance;
                selected = std::make_pair(std::cref(team), std::cref(player));
            }
        }
    }

    if (selected.has_value()) {
        dispatchClickedPlayer(selected->first, selected->second);
    }
}

void Map::dispatchClickedPlayer(const game::Team& team, const game::Player& player) const {
    if (!_dispatcher) {
        return;
    }
    _dispatcher->dispatch(events::PlayerClicked{
        .playerId = player.id(),
        .teamName = team.name(),
        .playerName = player.name(),
        .position = player.position(),
        .teamColor = team.color(),
        .textureId = player.textureId(),
    });
}
}  // namespace zappy::gui::graphics::scene
