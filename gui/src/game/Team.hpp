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

    void addPlayer(int id, raylib::rmath::Vector3 position, Player::cardinalPoint orientation) {
        _players.emplace_back(id, position, _name + std::to_string(id), orientation);
    }

    [[nodiscard]] const std::string& name() const { return _name; }
    [[nodiscard]] const std::vector<Player>& players() const { return _players; }

  protected:
  private:
    std::string _name;
    std::vector<Player> _players;
    std::vector<Egg> _eggs;
};
}  // namespace zappy::gui::game
