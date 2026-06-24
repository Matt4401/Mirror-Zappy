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

#include "Color.hpp"
#include "Player.hpp"
#include "rcore/Camera.hpp"
#include "rmath/Vector3.hpp"
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

float GameModel::getRotationAngle(Player::cardinalPoint orientation) {
    switch (orientation) {
        case Player::cardinalPoint::NORTH:
            return 180.0F;
        case Player::cardinalPoint::EAST:
            return 90.0F;
        case Player::cardinalPoint::SOUTH:
            return 0.0F;
        case Player::cardinalPoint::WEST:
            return 270.0F;
    }
    return 0.0F;
}

    void drawPlayer(raylib::rmath::Vector3 position, Player::cardinalPoint orientation,
                    const std::shared_ptr<raylib::rtextures::Texture2D>& texture, std::size_t level) const
    if (_camera.get().isVisibleFromCamera(position)) {
        if (texture && texture->valid()) {
            _playerModel.setMaterialTexture(0, MATERIAL_MAP_ALBEDO, *texture);
        } else if (_defaultPlayerTexture && _defaultPlayerTexture->valid()) {
            _playerModel.setMaterialTexture(0, MATERIAL_MAP_ALBEDO, *_defaultPlayerTexture);
        }
        _playerModel.drawModelEx(position, {0.0F, 1.0F, 0.0F}, getRotationAngle(orientation), PLAYER_SCALE,
                                 raylib::Color::White());
        if (level > 1 && level <= 8) {
            _armorModels.at(level - 2).drawModel(position, ARMOR_SCALE, raylib::Color::White());
        }
    }
}
void GameModel::drawEgg(raylib::rmath::Vector3 position, const raylib::Color tint) const {
    if (_camera.get().isVisibleFromCamera(position)) {
        _eggModel.drawModel(position, EGG_SCALE, tint);
    }
}
}  // namespace zappy::gui::game
