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
#include <utility>
#include <vector>

#include "Tile3D.hpp"
#include "events/EventDispatcher.hpp"
#include "game/GameModel.hpp"
#include "game/Player.hpp"
#include "game/Team.hpp"
#include "game/components/IObject.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::graphics::scene {
class Map {
  public:
    using ItemFunc = std::function<void(const game::IObject& object)>;
    using SelectedPlayer =
        std::pair<std::reference_wrapper<const game::Team>, std::reference_wrapper<const game::Player>>;

    static constexpr const char* TILE_MODEL_RESOURCE = "assets/minecraft-grass-block/source/Grass_Block.obj";

    static constexpr const char* DERAUMERE_MODEL_RESOURCE = "assets/red-ore.glb";
    static constexpr const char* LINEMATE_MODEL_RESOURCE = "assets/pink-ore.glb";
    static constexpr const char* SIBUR_MODEL_RESOURCE = "assets/green-ore.glb";
    static constexpr const char* PHIRAS_MODEL_RESOURCE = "assets/blue-ore.glb";
    static constexpr const char* THYSTAME_MODEL_RESOURCE = "assets/cyan-ore.glb";
    static constexpr const char* MENDIANE_MODEL_RESOURCE = "assets/orange-ore/scene.gltf";
    static constexpr const char* FOOD_TEXTURE_RESOURCE = "assets/food.glb";

    Map(int width, int height, std::shared_ptr<raylib::rcore::Camera> camera,
        std::shared_ptr<events::EventDispatcher> dispatcher = nullptr);
    ~Map() = default;
    Map(const Map& other) = delete;
    Map& operator=(const Map& other) = delete;
    Map(Map&& other) noexcept = default;
    Map& operator=(Map&& other) noexcept = default;

    void resize(int width, int height);

    void draw() const;
    void handleEvent(const raylib::rcore::Event& event);

  protected:
  private:
    void drawItems(const Tile3D& tile) const;
    void dispatchClickedPlayer(const game::Team& team, const game::Player& player) const;

    std::shared_ptr<raylib::rcore::Camera> _camera;
    std::shared_ptr<events::EventDispatcher> _dispatcher;
    std::vector<Tile3D> _tiles;
    std::vector<game::Team> _teams;
    game::GameModel _gameModel;
    int _width{0};
    int _height{0};
    std::map<std::string, ItemFunc> _itemDrawFunctions;
    raylib::rmodels::Model _tileModel{TILE_MODEL_RESOURCE};
    raylib::rmodels::Model _deraumereModel{DERAUMERE_MODEL_RESOURCE};
    raylib::rmodels::Model _linemateModel{LINEMATE_MODEL_RESOURCE};
    raylib::rmodels::Model _siburModel{SIBUR_MODEL_RESOURCE};
    raylib::rmodels::Model _phirasModel{PHIRAS_MODEL_RESOURCE};
    raylib::rmodels::Model _thystameModel{THYSTAME_MODEL_RESOURCE};
    raylib::rmodels::Model _mendianeModel{MENDIANE_MODEL_RESOURCE};
    raylib::rmodels::Model _foodModel{FOOD_TEXTURE_RESOURCE};

    static constexpr int MOUSE_LEFT_CLICK = 0;
};
}  // namespace zappy::gui::graphics::scene
