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
#include <utility>

#include "GuiEvents.hpp"
#include "Tile3D.hpp"
#include "WorldManager.hpp"
#include "events/EventDispatcher.hpp"
#include "game/GameModel.hpp"
#include "game/Player.hpp"
#include "game/Team.hpp"
#include "game/components/IObject.hpp"
#include "rcore/Camera.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::graphics::scene {
class Map {
  public:
    using ItemFunc = std::function<void(const game::IObject& object)>;
    using SelectedPlayer =
        std::pair<std::reference_wrapper<const game::Team>, std::reference_wrapper<const game::Player>>;

    static constexpr const char* TILE_MODEL_RESOURCE =
        "assets/models/environment/minecraft-grass-block/source/Grass_Block.obj";

    static constexpr const char* DERAUMERE_MODEL_RESOURCE = "assets/models/resources/red-ore.glb";
    static constexpr const char* LINEMATE_MODEL_RESOURCE = "assets/models/resources/pink-ore.glb";
    static constexpr const char* SIBUR_MODEL_RESOURCE = "assets/models/resources/green-ore.glb";
    static constexpr const char* PHIRAS_MODEL_RESOURCE = "assets/models/resources/blue-ore.glb";
    static constexpr const char* THYSTAME_MODEL_RESOURCE = "assets/models/resources/cyan-ore.glb";
    static constexpr const char* MENDIANE_MODEL_RESOURCE = "assets/models/resources/orange-ore/scene.gltf";
    static constexpr const char* FOOD_TEXTURE_RESOURCE = "assets/models/resources/food.glb";

    Map(raylib::rcore::Camera& camera, WorldManager& worldManager, events::EventDispatcher& dispatcher);
    ~Map();
    Map(const Map& other) = delete;
    Map& operator=(const Map& other) = delete;
    Map(Map&& other) = delete;
    Map& operator=(Map&& other) = delete;

    void draw() const;
    void handleEvent();

    void clearHoveredTile() { _hoveredTile = nullptr; }

  protected:
  private:
    void drawItems(const Tile3D& tile) const;
    void dispatchClickedPlayer(const game::Team& team, const game::Player& player) const;
    void dispatchClickedTile(const Tile3D& tile) const;
    void handleRequestCyclePlayer(const events::RequestCyclePlayer& e);

    const Tile3D* _hoveredTile{nullptr};
    std::reference_wrapper<raylib::rcore::Camera> _camera;
    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    std::reference_wrapper<WorldManager> _worldManager;
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
    events::EventDispatcher::EventToken _nameToken{0};
    events::EventDispatcher::EventToken _cycleToken{0};

    static constexpr int MOUSE_LEFT_CLICK = 0;
};
}  // namespace zappy::gui::graphics::scene
