/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Team
*/

#include "Team.hpp"

#include <memory>

#include "Egg.hpp"
#include "GameModel.hpp"
#include "Player.hpp"
#include "graphics/AssetManager.hpp"
#include "rtextures/Texture2D.hpp"

namespace zappy::gui::game {
void Team::draw(const GameModel& gameModel) const {
    for (const auto& player : _players) {
        std::shared_ptr<raylib::rtextures::Texture2D> tex = nullptr;
        if (!player.textureId().empty()) {
            tex = graphics::AssetManager::getInstance().getTexture(player.textureId());
        }
        gameModel.drawPlayer(player.position(), tex);
    }
    for (const auto& egg : _eggs) {
        gameModel.drawEgg(egg.position());
    }
}
}  // namespace zappy::gui::game
