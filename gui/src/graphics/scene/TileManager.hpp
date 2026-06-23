/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** TileManager
*/

#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Tile3D.hpp"
#include "game/ItemBag.hpp"
#include "game/components/Deraumere.hpp"
#include "game/components/Food.hpp"
#include "game/components/IObject.hpp"
#include "game/components/Linemate.hpp"
#include "game/components/Mendiane.hpp"
#include "game/components/Phiras.hpp"
#include "game/components/Sibur.hpp"
#include "game/components/Thystame.hpp"
#include "protocol/Commands.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::graphics::scene {
class TileManager {
  public:
    TileManager() = default;
    ~TileManager() = default;
    TileManager(const TileManager& other) = delete;
    TileManager& operator=(const TileManager& other) = delete;
    TileManager(TileManager&& other) = delete;
    TileManager& operator=(TileManager&& other) = delete;

    [[nodiscard]] int width() const { return _width; }
    [[nodiscard]] int height() const { return _height; }
    [[nodiscard]] const std::vector<Tile3D>& tiles() const { return _tiles; }
    [[nodiscard]] bool contains(int x, int y) const;
    [[nodiscard]] raylib::rmath::Vector3 tilePosition(int x, int y) const;
    [[nodiscard]] std::optional<std::reference_wrapper<const Tile3D>> tileAt(int x, int y) const;
    [[nodiscard]] std::optional<std::reference_wrapper<Tile3D>> mutableTileAt(int x, int y);

    void handleMapSize(const shared::protocol::server::Msz& command);
    void handleTileContent(const shared::protocol::server::Bct& command);
    void replaceResources(game::ItemBag& bag, const raylib::rmath::Vector3& position,
                          const std::array<int, 7>& quantities) const;
    void addResource(game::ItemBag& bag, const raylib::rmath::Vector3& position, int resourceId,
                     std::size_t quantity = 1) const;
    static void removeResource(game::ItemBag& bag, int resourceId, std::size_t quantity = 1);

  private:
    using ResourceFactory = std::function<std::unique_ptr<game::IObject>(const raylib::rmath::Vector3&)>;

    [[nodiscard]] static std::optional<std::string_view> resourceName(int resourceId);
    [[nodiscard]] std::unique_ptr<game::IObject> makeResource(int resourceId,
                                                              const raylib::rmath::Vector3& position) const;

    int _width{0};
    int _height{0};
    std::vector<Tile3D> _tiles;

    const std::unordered_map<int, ResourceFactory> _resourceFactories = {
        {0, [](const auto& position) { return std::make_unique<game::Food>(position); }},
        {1, [](const auto& position) { return std::make_unique<game::Linemate>(position); }},
        {2, [](const auto& position) { return std::make_unique<game::Deraumere>(position); }},
        {3, [](const auto& position) { return std::make_unique<game::Sibur>(position); }},
        {4, [](const auto& position) { return std::make_unique<game::Mendiane>(position); }},
        {5, [](const auto& position) { return std::make_unique<game::Phiras>(position); }},
        {6, [](const auto& position) { return std::make_unique<game::Thystame>(position); }},
    };
};
}  // namespace zappy::gui::graphics::scene
