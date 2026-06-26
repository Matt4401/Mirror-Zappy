/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GameModel
*/

#include "GameModel.hpp"

#include <raylib.h>
#include <rlgl.h>

#include <cmath>
#include <cstddef>
#include <memory>
#include <string>

#include "Color.hpp"
#include "rcore/Camera.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"
#include "rmodels/ModelAnimation.hpp"
#include "rtextures/Texture2D.hpp"

namespace zappy::gui::game {
GameModel::GameModel(raylib::rcore::Camera& camera)
    : _camera(camera), _alphaDiscardShader("", "assets/shaders/alpha_discard.fs") {
    if (_playerModel.model().materials != nullptr &&
        _playerModel.model().materials[0].maps !=  // NOLINT (cppcoreguidelines-pro-bounds-pointer-arithmetic)
            nullptr) {
        _defaultPlayerTexture = std::make_shared<raylib::rtextures::Texture2D>(
            _playerModel  // NOLINT (cppcoreguidelines-pro-bounds-pointer-arithmetic)
                .model()
                .materials[0]
                .maps[MATERIAL_MAP_ALBEDO]
                .texture,
            false);
    }

    for (int i = 0; i < _playerModel.model().materialCount; i++) {
        // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
        _playerModel.model().materials[i].shader = _alphaDiscardShader.shader();
    }
    for (auto& _armorModel : _armorModels) {
        for (int j = 0; j < _armorModel.model().materialCount; j++) {
            // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
            _armorModel.model().materials[j].shader = _alphaDiscardShader.shader();
        }
    }
}

int GameModel::getAnimationIndexFromAction(const Player::Action action) {
    switch (action) {
        case Player::Action::WALK:
            return 0;
        case Player::Action::INCANTATION:
            return 1;
        case Player::Action::FORK:
            return 2;
        default:
            return 0;
    }
}

void GameModel::updateAnimationIfValid(const raylib::rmodels::ModelAnimation& anim, raylib::rmodels::Model& model,
                                       const int animIndex, const int animFrame) {
    if (!anim.valid() || animIndex < 0) {
        return;
    }
    if (animIndex != 0 && anim.frameCount(0) > 0) {
        anim.update(model, 0.0F, 0);
    }
    if (anim.frameCount(animIndex) > 0) {
        anim.update(model, static_cast<float>(animFrame % anim.frameCount(animIndex)), animIndex);
    }
}

void GameModel::drawPlayer(raylib::rmath::Vector3 position, float rotationAngle, Player::Action action, int animFrame,
                           const std::shared_ptr<raylib::rtextures::Texture2D>& texture, std::size_t level) const {
    if (!_camera.get().isVisibleFromCamera(position)) {
        return;
    }

    const int animIndex = getAnimationIndexFromAction(action);
    updateAnimationIfValid(_playerAnim, _playerModel, animIndex, animFrame);

    if (texture && texture->valid()) {
        _playerModel.setMaterialTexture(0, MATERIAL_MAP_ALBEDO, *texture);
    } else if (_defaultPlayerTexture && _defaultPlayerTexture->valid()) {
        _playerModel.setMaterialTexture(0, MATERIAL_MAP_ALBEDO, *_defaultPlayerTexture);
    }

    _playerModel.drawModelEx(position, {0.0F, 1.0F, 0.0F}, rotationAngle, PLAYER_SCALE, raylib::Color::White());

    if (level > 1 && level <= 8) {
        auto& armorAnim = _armorAnims.at(level - 2);
        updateAnimationIfValid(armorAnim, _armorModels.at(level - 2), animIndex, animFrame);

        raylib::Color armorTint = raylib::Color::White();
        if (level == 2) {
            armorTint = raylib::Color(139, 69, 19, 255);
        }
        _armorModels.at(level - 2).drawModelEx(position, {0.0F, 1.0F, 0.0F}, rotationAngle,
                                               {ARMOR_SCALE, ARMOR_SCALE, ARMOR_SCALE}, armorTint);
    }
}
void GameModel::drawEgg(raylib::rmath::Vector3 position, const raylib::Color tint) const {
    if (_camera.get().isVisibleFromCamera(position)) {
        _eggModel.drawModel(position, EGG_SCALE, tint);
    }
}
}  // namespace zappy::gui::game
