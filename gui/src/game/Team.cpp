/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Team
*/

#include "Team.hpp"
#include "GameModel.hpp"
#include "Egg.hpp"
#include <rmath/Vector3.hpp>
#include <string>
#include "server/src/game/Player.hpp"
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

    void Team::playerHatched(raylib::rmath::Vector3 position, server::game::cardinalPoint orientation) {
        _players.emplace_back(position, _name + std::to_string(_players.size()), orientation);
        _eggs.erase(_eggs.begin());
    }
}
