/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Team
*/

#include "Team.hpp"

#include <rmath/Vector3.hpp>

#include "Egg.hpp"
#include "GameModel.hpp"
#include "Player.hpp"

namespace zappy::gui::game {
void Team::draw(const GameModel& gameModel) const {
    for (const auto& player : _players) {
        gameModel.drawPlayer(player.position());
    }
    for (const auto& egg : _eggs) {
        gameModel.drawEgg(egg.position());
    }
}
}  // namespace zappy::gui::game
