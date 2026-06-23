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
#include "WorldManager.hpp"
#include "events/EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "game/Player.hpp"
#include "game/Team.hpp"
#include "game/components/IObject.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"

namespace zappy::gui::graphics::scene {
Map::Map(std::shared_ptr<raylib::rcore::Camera> camera, std::reference_wrapper<WorldManager> worldManager,
         std::shared_ptr<events::EventDispatcher> dispatcher)
    : _camera(std::move(camera)), _dispatcher(std::move(dispatcher)), _worldManager(worldManager), _gameModel(_camera) {
    _itemDrawFunctions["Deraumere"] = [this](const game::IObject& object) { object.draw(_deraumereModel); };
    _itemDrawFunctions["Linemate"] = [this](const game::IObject& object) { object.draw(_linemateModel); };
    _itemDrawFunctions["Sibur"] = [this](const game::IObject& object) { object.draw(_siburModel); };
    _itemDrawFunctions["Phiras"] = [this](const game::IObject& object) { object.draw(_phirasModel); };
    _itemDrawFunctions["Thystame"] = [this](const game::IObject& object) { object.draw(_thystameModel); };
    _itemDrawFunctions["Mendiane"] = [this](const game::IObject& object) { object.draw(_mendianeModel); };
    _itemDrawFunctions["Food"] = [this](const game::IObject& object) { object.draw(_foodModel); };

    if (_dispatcher) {
        _nameToken = _dispatcher->subscribe<events::PlayerNameChanged>([this](const events::PlayerNameChanged& e) {
            _worldManager.get().updatePlayerName(e.playerId, e.newName);
        });
    }
}

Map::~Map() {
    if (_dispatcher && _nameToken != 0) {
        _dispatcher->unsubscribe<events::PlayerNameChanged>(_nameToken);
    }
}

void Map::draw() const {
    for (const auto& tile : _worldManager.get().tiles()) {
        if (_camera->isVisibleFromCamera(tile.position())) {
            tile.draw(_tileModel);
            if (tile.itemBag().hasItems()) {
                drawItems(tile);
            }
        }
    }
    for (const auto& team : _worldManager.get().teams()) {
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

void Map::handleEvent(const raylib::rcore::Event& /*event*/) {
    if (!raylib::rcore::Event::isMouseButtonPressed(MOUSE_LEFT_CLICK) || !_camera) {
        return;
    }

    const auto ray = raylib::rcore::Event::mouseRay(*_camera);
    std::optional<SelectedPlayer> selected;
    float nearestDistance = std::numeric_limits<float>::max();

    for (const auto& team : _worldManager.get().teams()) {
        for (const auto& player : team.players()) {
            if (!_camera->isVisibleFromCamera(player.position())) {
                continue;
            }
            const auto collision = ray.collision(player.boundingBox());
            if (collision.hit && collision.distance < nearestDistance) {
                nearestDistance = collision.distance;
                selected = std::make_pair(std::cref(team), std::cref(player));
            }
        }
    }

    if (selected.has_value()) {
        dispatchClickedPlayer(selected->first, selected->second);
        return;
    }

    const Tile3D* selectedTile = nullptr;
    nearestDistance = std::numeric_limits<float>::max();

    for (const auto& tile : _worldManager.get().tiles()) {
        if (!_camera->isVisibleFromCamera(tile.position())) {
            continue;
        }
        const auto collision = ray.collision(tile.boundingBox());
        if (collision.hit && collision.distance < nearestDistance) {
            nearestDistance = collision.distance;
            selectedTile = &tile;
        }
    }

    if (selectedTile != nullptr) {
        dispatchClickedTile(*selectedTile);
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

void Map::dispatchClickedTile(const Tile3D& tile) const {
    if (!_dispatcher) {
        return;
    }
    _dispatcher->dispatch(events::TileClicked{
        .x = tile.gridX(),
        .y = tile.gridY(),
    });
}
}  // namespace zappy::gui::graphics::scene
