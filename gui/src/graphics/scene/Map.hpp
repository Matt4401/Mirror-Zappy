/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Map
*/

#pragma once
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Tile3D.hpp"
#include "game/GameModel.hpp"
#include "game/Team.hpp"
#include "game/components/IObject.hpp"
#include "rcore/Camera.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::graphics::scene {
class Map {
  public:
    using ItemFunc = std::function<void(const game::IObject& object)>;

    static constexpr const char* TILE_MODEL_RESOURCE = "assets/minecraft-grass-block/source/Grass_Block.obj";

    static constexpr const char* DERAUMERE_MODEL_RESOURCE = "assets/red-ore.glb";
    static constexpr const char* LINEMATE_MODEL_RESOURCE = "assets/pink-ore.glb";
    static constexpr const char* SIBUR_MODEL_RESOURCE = "assets/green-ore.glb";
    static constexpr const char* PHIRAS_MODEL_RESOURCE = "assets/blue-ore.glb";
    static constexpr const char* THYSTAME_MODEL_RESOURCE = "assets/cyan-ore.glb";
    static constexpr const char* MENDIANE_MODEL_RESOURCE = "assets/orange-ore/scene.gltf";
    static constexpr const char* FOOD_TEXTURE_RESOURCE = "assets/food.glb";

    Map(int width, int height, std::shared_ptr<raylib::rcore::Camera> camera);
    ~Map() = default;
    Map(const Map& other) = delete;
    Map& operator=(const Map& other) = delete;
    Map(Map&& other) noexcept = default;
    Map& operator=(Map&& other) noexcept = default;

    void resize(int width, int height);

    void draw() const;

  protected:
  private:
    void drawItems(const Tile3D& tile) const;

    std::shared_ptr<raylib::rcore::Camera> _camera;
    std::vector<Tile3D> _tiles;
    std::vector<game::Team> _teams;
    game::GameModel _gameModel;
    std::map<std::string, ItemFunc> _itemDrawFunctions;
    raylib::rmodels::Model _tileModel{TILE_MODEL_RESOURCE};
    raylib::rmodels::Model _deraumereModel{DERAUMERE_MODEL_RESOURCE};
    raylib::rmodels::Model _linemateModel{LINEMATE_MODEL_RESOURCE};
    raylib::rmodels::Model _siburModel{SIBUR_MODEL_RESOURCE};
    raylib::rmodels::Model _phirasModel{PHIRAS_MODEL_RESOURCE};
    raylib::rmodels::Model _thystameModel{THYSTAME_MODEL_RESOURCE};
    raylib::rmodels::Model _mendianeModel{MENDIANE_MODEL_RESOURCE};
    raylib::rmodels::Model _foodModel{FOOD_TEXTURE_RESOURCE};
};
}  // namespace zappy::gui::graphics::scene
