/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GameModel
*/

#include "GameModel.hpp"

#include "Color.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::game {
void GameModel::drawPlayer(raylib::rmath::Vector3 position) const {
    if (_camera->isVisibleFromCamera(position)) {
        _playerModel.drawModel(position, PLAYER_SCALE, raylib::Color::White());
    }
}
void GameModel::drawEgg(raylib::rmath::Vector3 position) const {
    if (_camera->isVisibleFromCamera(position)) {
        _eggModel.drawModel(position, EGG_SCALE, raylib::Color::White());
    }
}
}  // namespace zappy::gui::game
