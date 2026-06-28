/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Map
*/

#include "Map.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "Color.hpp"
#include "SettingsManager.hpp"
#include "Tile3D.hpp"
#include "WorldManager.hpp"
#include "events/EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "game/Player.hpp"
#include "game/Team.hpp"
#include "game/components/IObject.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Ray.hpp"
#include "rshapes/Shapes.hpp"

namespace zappy::gui::graphics::scene {
Map::Map(raylib::rcore::Camera& camera, WorldManager& worldManager, events::EventDispatcher& dispatcher,
         SettingsManager& settingsManager)
    : _camera(camera),
      _dispatcher(dispatcher),
      _worldManager(worldManager),
      _settingsManager(settingsManager),
      _gameModel(_camera) {
    _itemDrawFunctions["Deraumere"] = [this](const game::IObject& object) { object.draw(_deraumereModel); };
    _itemDrawFunctions["Linemate"] = [this](const game::IObject& object) { object.draw(_linemateModel); };
    _itemDrawFunctions["Sibur"] = [this](const game::IObject& object) { object.draw(_siburModel); };
    _itemDrawFunctions["Phiras"] = [this](const game::IObject& object) { object.draw(_phirasModel); };
    _itemDrawFunctions["Thystame"] = [this](const game::IObject& object) { object.draw(_thystameModel); };
    _itemDrawFunctions["Mendiane"] = [this](const game::IObject& object) { object.draw(_mendianeModel); };
    _itemDrawFunctions["Food"] = [this](const game::IObject& object) { object.draw(_foodModel); };

    _nameToken = _dispatcher.get().subscribe<events::PlayerNameChanged>(
        [this](const events::PlayerNameChanged& e) { _worldManager.get().updatePlayerName(e.playerId, e.newName); });

    _cycleToken = _dispatcher.get().subscribe<events::RequestCyclePlayer>(
        [this](const events::RequestCyclePlayer& e) { handleRequestCyclePlayer(e); });
}

Map::~Map() {
    if (_nameToken != 0) {
        _dispatcher.get().unsubscribe<events::PlayerNameChanged>(_nameToken);
    }
    if (_cycleToken != 0) {
        _dispatcher.get().unsubscribe<events::RequestCyclePlayer>(_cycleToken);
    }
}

void Map::draw() const {
    const auto& settings = _settingsManager.get().getSettings();

    for (const auto& tile : _worldManager.get().tiles()) {
        if (_camera.get().isVisibleFromCamera(tile.position())) {
            if (settings.showTiles) {
                tile.draw(_tileModel);
            }
            if (tile.itemBag().hasItems()) {
                drawItems(tile);
            }
        }
    }
    if (settings.showPlayers) {
        for (const auto& team : _worldManager.get().teams()) {
            team.draw(_gameModel);
        }
    }
    if (_hoveredTile != nullptr && settings.showTiles) {
        raylib::rshapes::Shapes::drawThickBoundingBox(_hoveredTile->boundingBox(_tileModel.getBoundingBox()),
                                                      raylib::Color::White(), 0.05F);
    }
}

void Map::drawItems(const Tile3D& tile) const {
    const auto& settings = _settingsManager.get().getSettings();
    for (const auto& item : tile.itemBag().items()) {
        if (!settings.showFood && item.object->name() == "Food") {
            continue;
        }
        if (!settings.showMinerals && item.object->name() != "Food") {
            continue;
        }

        auto it = _itemDrawFunctions.find(item.object->name());
        if (it != _itemDrawFunctions.end()) {
            it->second(*item.object);
        }
    }
}

void Map::updateHoveredTile(const raylib::rcore::Ray& ray, float& nearestDistance) {
    for (const auto& tile : _worldManager.get().tiles()) {
        if (!_camera.get().isVisibleFromCamera(tile.position())) {
            continue;
        }
        const auto collision = ray.collision(tile.boundingBox(_tileModel.getBoundingBox()));
        if (collision.hit && collision.distance < nearestDistance) {
            nearestDistance = collision.distance;
            _hoveredTile = &tile;
        }
    }
}

std::optional<Map::SelectedPlayer> Map::getSelectedPlayer(const raylib::rcore::Ray& ray, float& nearestDistance) const {
    std::optional<SelectedPlayer> selected;
    for (const auto& team : _worldManager.get().teams()) {
        for (const auto& player : team.players()) {
            if (!_camera.get().isVisibleFromCamera(player.position())) {
                continue;
            }
            const auto collision = ray.collision(player.boundingBox());
            if (collision.hit && collision.distance < nearestDistance) {
                nearestDistance = collision.distance;
                selected = std::make_pair(std::cref(team), std::cref(player));
            }
        }
    }
    return selected;
}

void Map::handleEvent() {
    const auto& settings = _settingsManager.get().getSettings();
    _hoveredTile = nullptr;
    const auto ray = raylib::rcore::Event::mouseRay(_camera.get());
    float nearestDistance = std::numeric_limits<float>::max();

    if (settings.showTiles) {
        updateHoveredTile(ray, nearestDistance);
    }

    if (!raylib::rcore::Event::isMouseButtonPressed(MOUSE_LEFT_CLICK)) {
        return;
    }

    nearestDistance = std::numeric_limits<float>::max();
    std::optional<SelectedPlayer> selected;

    if (settings.showPlayers) {
        selected = getSelectedPlayer(ray, nearestDistance);
    }

    if (selected.has_value()) {
        dispatchClickedPlayer(selected->first, selected->second);
        return;
    }

    if (_hoveredTile != nullptr) {
        dispatchClickedTile(*_hoveredTile);
    }
}

void Map::dispatchClickedPlayer(const game::Team& team, const game::Player& player) const {
    _dispatcher.get().dispatch(events::PlayerClicked{
        .playerId = player.id(),
        .teamName = team.name(),
        .playerName = player.name(),
        .position = player.position(),
        .teamColor = team.color(),
        .textureId = player.textureId(),
    });
}

void Map::dispatchClickedTile(const Tile3D& tile) const {
    const auto gridPosition = tile.gridPosition();
    _dispatcher.get().dispatch(events::TileClicked{
        .x = gridPosition.x,
        .y = gridPosition.y,
    });
}

void Map::handleRequestCyclePlayer(const events::RequestCyclePlayer& e) {
    std::vector<std::pair<std::reference_wrapper<const game::Team>, std::reference_wrapper<const game::Player>>>
        allPlayers;
    for (const auto& team : _worldManager.get().teams()) {
        for (const auto& player : team.players()) {
            allPlayers.emplace_back(std::cref(team), std::cref(player));
        }
    }
    if (allPlayers.empty()) {
        return;
    }

    std::ranges::sort(allPlayers,
                      [](const auto& a, const auto& b) { return a.second.get().id() < b.second.get().id(); });

    auto it = std::ranges::find_if(allPlayers,
                                   [&e](const auto& pair) { return pair.second.get().id() == e.currentPlayerId; });

    std::size_t currentIndex = 0;
    if (it != allPlayers.end()) {
        currentIndex = static_cast<std::size_t>(std::distance(allPlayers.begin(), it));
    }

    std::size_t nextIndex = currentIndex;
    if (e.direction < 0) {
        nextIndex = (currentIndex == 0) ? allPlayers.size() - 1 : currentIndex - 1;
    } else if (e.direction > 0) {
        nextIndex = (currentIndex + 1 >= allPlayers.size()) ? 0 : currentIndex + 1;
    }

    dispatchClickedPlayer(allPlayers.at(nextIndex).first.get(), allPlayers.at(nextIndex).second.get());
}
}  // namespace zappy::gui::graphics::scene
