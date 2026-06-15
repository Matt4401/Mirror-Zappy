/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Map
*/

#pragma once
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "Tile3D.hpp"
#include "game/components/IObject.hpp"
#include "rcore/Camera.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::graphics::scene {
class Map {
  public:
    using ItemFunc = std::function<void(const game::IObject& object)>;

    static constexpr const char* TILE_MODEL_RESOURCE = "assets/minecraft-grass-block/source/Grass_Block.obj";

    // NOT FINAL ASSET PATH, JUST FOR TESTING
    static constexpr const char* DERAUMERE_MODEL_RESOURCE = "assets/red-ore.glb";
    static constexpr const char* LINEMATE_MODEL_RESOURCE = "assets/pink-ore.glb";

    Map(int width, int height);
    ~Map() = default;
    Map(const Map& other) = delete;
    Map& operator=(const Map& other) = delete;
    Map(Map&& other) noexcept = default;
    Map& operator=(Map&& other) noexcept = default;

    void resize(int width, int height);

    void draw(const raylib::rcore::Camera& camera) const;

  protected:
  private:
    void drawItems(const Tile3D& tile) const;

    std::vector<Tile3D> _tiles;
    std::map<std::string, ItemFunc> _itemDrawFunctions;
    raylib::rmodels::Model _tileModel{TILE_MODEL_RESOURCE};
    raylib::rmodels::Model _deraumereModel{DERAUMERE_MODEL_RESOURCE};
    raylib::rmodels::Model _linemateModel{LINEMATE_MODEL_RESOURCE};
};
}  // namespace zappy::gui::graphics::scene
