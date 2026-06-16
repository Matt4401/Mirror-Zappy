/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GameModel
*/

#pragma once
#include <memory>

#include "rcore/Camera.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::game {
class GameModel {
  public:
    static constexpr auto PLAYER_SCALE = 0.1F;
    static constexpr auto PLAYER_MODEL_RESOURCE = "assets/jeffrey/scene.gltf";
    static constexpr auto EGG_MODEL_RESOURCE = "assets/minecraft_dragon_egg/scene.gltf";
    static constexpr auto EGG_SCALE = 0.6F;

    GameModel(std::shared_ptr<raylib::rcore::Camera>& camera) : _camera(camera) {}
    ~GameModel() = default;
    GameModel(const GameModel& other) = delete;
    GameModel& operator=(const GameModel& other) = delete;
    GameModel(GameModel&& other) noexcept = default;
    GameModel& operator=(GameModel&& other) noexcept = default;

    void drawPlayer(raylib::rmath::Vector3 position) const;
    void drawEgg(raylib::rmath::Vector3 position) const;

  protected:
  private:
    std::shared_ptr<raylib::rcore::Camera> _camera;
    raylib::rmodels::Model _playerModel{PLAYER_MODEL_RESOURCE};
    raylib::rmodels::Model _eggModel{EGG_MODEL_RESOURCE};
};
}  // namespace zappy::gui::game
