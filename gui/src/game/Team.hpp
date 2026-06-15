/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Team
*/

#pragma once
#include <rmath/Vector3.hpp>
#include <string>
#include <utility>
#include <vector>

#include "Egg.hpp"
#include "GameModel.hpp"
#include "Player.hpp"
#include "server/src/game/Player.hpp"

namespace zappy::gui::game {
class Team {
  public:
    Team(std::string& name, int slot) : _name(std::move(name)), _eggs(slot) {};
    ~Team() = default;
    Team(const Team& other) = delete;
    Team& operator=(const Team& other) = delete;
    Team(Team&& other) noexcept = default;
    Team& operator=(Team&& other) noexcept = default;

    void draw(const GameModel& gameModel) const;
    void playerHatched(raylib::rmath::Vector3 position, server::game::cardinalPoint orientation);

    void addPlayer(raylib::rmath::Vector3 position = {10.0F, 12.0F, 0.0F}) {
        _players.emplace_back(position, _name + std::to_string(_players.size()), Player::cardinalPoint::NORTH);
    }  // TEMPORARY FUNCTION, JUST FOR TESTING

  protected:
  private:
    std::string _name;
    std::vector<Player> _players;
    std::vector<Egg> _eggs;
};
}  // namespace zappy::gui::game
