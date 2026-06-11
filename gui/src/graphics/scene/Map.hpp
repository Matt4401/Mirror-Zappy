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
#include "components/IObject.hpp"
#include "rcore/Camera.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::graphics::scene {
class Map {
  public:
    using ItemFunc = std::function<void(const IObject& object)>;

    static constexpr int TILE_SIZE = 2;
    static constexpr const char* TILE_MODEL_RESOURCE = "assets/minecraft-grass-block/source/Grass_Block.obj";

    // NOT FINAL ASSET PATH, JUST FOR TESTING
    static constexpr const char* DERAUMERE_MODEL_RESOURCE = "assets/ciao_kombucha_gloww/scene.gltf";

    Map(int width, int height);
    ~Map() = default;
    Map(const Map& other) = delete;
    Map& operator=(const Map& other) = delete;
    Map(Map&& other) noexcept = default;
    Map& operator=(Map&& other) noexcept = default;

    void draw(const raylib::rcore::Camera& camera) const;

  protected:
  private:
    void drawItems(const Tile3D& tile) const;
    void drawDeraumere(const IObject& object) const;

    std::vector<Tile3D> _tiles;
    std::map<std::string, ItemFunc> _itemDrawFunctions;
    raylib::rmodels::Model _tileModel{TILE_MODEL_RESOURCE};
    raylib::rmodels::Model _deraumereModel{DERAUMERE_MODEL_RESOURCE};
};
}  // namespace zappy::gui::graphics::scene
