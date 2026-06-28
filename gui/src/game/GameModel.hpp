/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GameModel
*/

#pragma once
#include <array>
#include <cstddef>
#include <functional>
#include <memory>

#include "Color.hpp"
#include "Player.hpp"
#include "rcore/Camera.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"
#include "rmodels/ModelAnimation.hpp"
#include "rmodels/Shader.hpp"
#include "rtextures/Texture2D.hpp"

namespace zappy::gui::game {
class GameModel {
  public:
    static constexpr raylib::rmath::Vector3 PLAYER_SCALE = {2.0F, 2.0F, 2.0F};
    static constexpr auto PLAYER_MODEL_RESOURCE = "assets/models/player/player.gltf";
    static constexpr auto EGG_MODEL_RESOURCE = "assets/models/environment/minecraft_dragon_egg/scene.gltf";
    static constexpr auto EGG_SCALE = 0.6F;
    static constexpr auto EGG_TINT_STRENGTH = 0.75F;
    static constexpr auto ARMOR_SCALE = 2.1F;

    GameModel(raylib::rcore::Camera& camera);
    ~GameModel() = default;
    GameModel(const GameModel& other) = delete;
    GameModel& operator=(const GameModel& other) = delete;
    GameModel(GameModel&& other) noexcept = default;
    GameModel& operator=(GameModel&& other) noexcept = default;

    void drawPlayer(raylib::rmath::Vector3 position, float rotationAngle, Player::Action action, int animFrame,
                    const std::shared_ptr<raylib::rtextures::Texture2D>& texture = nullptr,
                    std::size_t level = 1) const;
    void drawEgg(raylib::rmath::Vector3 position, raylib::Color tint) const;

  protected:
  private:
    static int getAnimationIndexFromAction(Player::Action action);
    static void updateAnimationIfValid(const raylib::rmodels::ModelAnimation& anim, raylib::rmodels::Model& model,
                                       int animIndex, int animFrame);
    std::reference_wrapper<raylib::rcore::Camera> _camera;
    mutable raylib::rmodels::Model _playerModel{PLAYER_MODEL_RESOURCE};
    mutable raylib::rmodels::ModelAnimation _playerAnim{PLAYER_MODEL_RESOURCE};
    std::shared_ptr<raylib::rtextures::Texture2D> _defaultPlayerTexture;
    mutable raylib::rmodels::Model _eggModel{EGG_MODEL_RESOURCE};
    mutable std::array<raylib::rmodels::Model, 7> _armorModels{
        raylib::rmodels::Model{"assets/models/player/armor/leather.gltf"},
        raylib::rmodels::Model{"assets/models/player/armor/chainmail.gltf"},
        raylib::rmodels::Model{"assets/models/player/armor/copper.gltf"},
        raylib::rmodels::Model{"assets/models/player/armor/iron.gltf"},
        raylib::rmodels::Model{"assets/models/player/armor/gold.gltf"},
        raylib::rmodels::Model{"assets/models/player/armor/diamond.gltf"},
        raylib::rmodels::Model{"assets/models/player/armor/netherite.gltf"}};
    mutable std::array<raylib::rmodels::ModelAnimation, 7> _armorAnims{
        raylib::rmodels::ModelAnimation{"assets/models/player/armor/leather.gltf"},
        raylib::rmodels::ModelAnimation{"assets/models/player/armor/chainmail.gltf"},
        raylib::rmodels::ModelAnimation{"assets/models/player/armor/copper.gltf"},
        raylib::rmodels::ModelAnimation{"assets/models/player/armor/iron.gltf"},
        raylib::rmodels::ModelAnimation{"assets/models/player/armor/gold.gltf"},
        raylib::rmodels::ModelAnimation{"assets/models/player/armor/diamond.gltf"},
        raylib::rmodels::ModelAnimation{"assets/models/player/armor/netherite.gltf"}};
    raylib::rmodels::Shader _alphaDiscardShader;

    static constexpr std::size_t MaxMaterialMaps = 12;
};
}  // namespace zappy::gui::game
