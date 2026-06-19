/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Team
*/

#pragma once
#include <cstddef>
#include <rmath/Vector3.hpp>
#include <string>
#include <vector>

#include "Color.hpp"
#include "Egg.hpp"
#include "GameModel.hpp"
#include "Player.hpp"

namespace zappy::gui::game {
class Team {
  public:
    Team(std::string& name, std::size_t slot) : _name(name), _eggs(slot) {};
    ~Team() = default;
    Team(const Team& other) = delete;
    Team& operator=(const Team& other) = delete;
    Team(Team&& other) noexcept = default;
    Team& operator=(Team&& other) noexcept = default;

    void draw(const GameModel& gameModel) const;

    void addPlayer(raylib::rmath::Vector3 position = {10.0F, 12.0F, 0.0F}) {
        _players.emplace_back(position, _name + std::to_string(_players.size()), Player::cardinalPoint::NORTH, _name,
                              raylib::Color(255, 0, 0, 255));
    }  // TEMPORARY FUNCTION, JUST FOR TESTING

  protected:
  private:
    std::string _name;
    std::vector<Player> _players;
    std::vector<Egg> _eggs;
};
}  // namespace zappy::gui::game
