/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** TileManager
*/

#include "TileManager.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string_view>
#include <utility>

#include "Tile3D.hpp"
#include "game/ItemBag.hpp"
#include "game/components/IObject.hpp"
#include "protocol/Commands.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::graphics::scene {
void TileManager::handleMapSize(const shared::protocol::server::Msz& command) {
    _width = std::max(command.width, 0);
    _height = std::max(command.height, 0);
    _tiles.clear();
    _tiles.reserve(static_cast<std::size_t>(_width) * static_cast<std::size_t>(_height));

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            const Tile3DPosition position{.x = x, .y = y};
            _tiles.emplace_back(position, tilePosition(position));
        }
    }
}

void TileManager::handleTileContent(const shared::protocol::server::Bct& command) {
    const auto tile = tileAt({.x = command.x, .y = command.y});
    if (!tile.has_value()) {
        return;
    }

    auto& bag = tile->get().itemBag();
    replaceResources(bag, tile->get().position(),
                     {command.food, command.linemate, command.deraumere, command.sibur, command.mendiane,
                      command.phiras, command.thystame});
}

bool TileManager::contains(const Tile3DPosition position) const {
    return position.x >= 0 && position.y >= 0 && position.x < _width && position.y < _height;
}

raylib::rmath::Vector3 TileManager::tilePosition(const Tile3DPosition position) const {
    const float offsetX = static_cast<float>(_width - 1) * Tile3D::TILE_SIZE * 0.5F;
    const float offsetZ = static_cast<float>(_height - 1) * Tile3D::TILE_SIZE * 0.5F;
    return {(static_cast<float>(position.x) * Tile3D::TILE_SIZE) - offsetX, 0.0F,
            (static_cast<float>(position.y) * Tile3D::TILE_SIZE) - offsetZ};
}

std::optional<std::reference_wrapper<const Tile3D>> TileManager::tileAt(const Tile3DPosition position) const {
    if (!contains(position)) {
        return std::nullopt;
    }
    const auto index = (static_cast<std::size_t>(position.y) * static_cast<std::size_t>(_width)) +
                       static_cast<std::size_t>(position.x);
    return std::cref(_tiles.at(index));
}

std::optional<std::reference_wrapper<Tile3D>> TileManager::tileAt(const Tile3DPosition position) {
    if (!contains(position)) {
        return std::nullopt;
    }
    const auto index = (static_cast<std::size_t>(position.y) * static_cast<std::size_t>(_width)) +
                       static_cast<std::size_t>(position.x);
    return std::ref(_tiles.at(index));
}

std::optional<std::string_view> TileManager::resourceName(const int resourceId) {
    if (resourceId < 0 || std::cmp_greater_equal(resourceId, ResourceNames.size())) {
        return std::nullopt;
    }
    return ResourceNames.at(static_cast<std::size_t>(resourceId));
}

std::unique_ptr<game::IObject> TileManager::makeResource(const int resourceId,
                                                         const raylib::rmath::Vector3& position) const {
    if (const auto factory = _resourceFactories.find(resourceId); factory != _resourceFactories.end()) {
        return factory->second(position);
    }
    return nullptr;
}

void TileManager::replaceResources(game::ItemBag& bag, const raylib::rmath::Vector3& position,
                                   const std::array<int, ResourceCount>& quantities) const {
    bag.clear();
    for (std::size_t resourceId = 0; resourceId < quantities.size(); ++resourceId) {
        if (quantities.at(resourceId) > 0) {
            addResource(bag, position, static_cast<int>(resourceId),
                        static_cast<std::size_t>(quantities.at(resourceId)));
        }
    }
}

void TileManager::addResource(game::ItemBag& bag, const raylib::rmath::Vector3& position, const int resourceId,
                              const std::size_t quantity) const {
    if (auto resource = makeResource(resourceId, position); resource) {
        bag.addItem(std::move(resource), quantity);
    }
}

void TileManager::removeResource(game::ItemBag& bag, const int resourceId, const std::size_t quantity) {
    if (const auto name = resourceName(resourceId); name.has_value()) {
        bag.removeItem(name.value(), quantity);
    }
}
}  // namespace zappy::gui::graphics::scene
