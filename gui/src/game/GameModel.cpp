/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GameModel
*/

#include "GameModel.hpp"

#include <raylib.h>

#include <cstddef>
#include <memory>
#include <span>

#include "Color.hpp"
#include "rcore/Camera.hpp"
#include "rmath/Vector3.hpp"
#include "rtextures/Texture2D.hpp"

namespace zappy::gui::game {
GameModel::GameModel(raylib::rcore::Camera& camera)
    : _camera(camera), _alphaDiscardShader("", "assets/shaders/alpha_discard.fs") {
    if (_playerModel.model().materials != nullptr) {
        std::span<::Material> materials(_playerModel.model().materials, _playerModel.model().materialCount);
        if (!materials.empty() && materials.front().maps != nullptr) {
            std::span<::MaterialMap> const maps(materials.front().maps, MaxMaterialMaps);
            _defaultPlayerTexture =
                std::make_shared<raylib::rtextures::Texture2D>(maps[MATERIAL_MAP_ALBEDO].texture, false);
        }

        for (auto& material : materials) {
            material.shader = _alphaDiscardShader.shader();
        }
    }

    for (auto& _armorModel : _armorModels) {
        if (_armorModel.model().materials != nullptr) {
            std::span<::Material> materials(_armorModel.model().materials, _armorModel.model().materialCount);
            for (auto& material : materials) {
                material.shader = _alphaDiscardShader.shader();
            }
        }
    }
}

void GameModel::drawPlayer(raylib::rmath::Vector3 position, const float rotationAngle,
                           const std::shared_ptr<raylib::rtextures::Texture2D>& texture, std::size_t level) const {
    if (_camera.get().isVisibleFromCamera(position)) {
        if (texture && texture->valid()) {
            _playerModel.setMaterialTexture(0, MATERIAL_MAP_ALBEDO, *texture);
        } else if (_defaultPlayerTexture && _defaultPlayerTexture->valid()) {
            _playerModel.setMaterialTexture(0, MATERIAL_MAP_ALBEDO, *_defaultPlayerTexture);
        }
        _playerModel.drawModelEx(position, {0.0F, 1.0F, 0.0F}, rotationAngle, PLAYER_SCALE, raylib::Color::White());
        if (level > 1 && level <= 8) {
            _armorModels.at(level - 2).drawModelEx(position, {0.0F, 1.0F, 0.0F}, rotationAngle,
                                                   {ARMOR_SCALE, ARMOR_SCALE, ARMOR_SCALE}, raylib::Color::White());
        }
    }
}
void GameModel::drawEgg(raylib::rmath::Vector3 position, const raylib::Color tint) const {
    if (_camera.get().isVisibleFromCamera(position)) {
        _eggModel.drawModel(position, EGG_SCALE, tint);
    }
}
}  // namespace zappy::gui::game
