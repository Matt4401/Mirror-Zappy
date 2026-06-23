/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Team
*/

#pragma once
#include <cstddef>
#include <functional>
#include <optional>
#include <rmath/Vector3.hpp>
#include <string>
#include <utility>
#include <vector>

#include "Color.hpp"
#include "Egg.hpp"
#include "GameModel.hpp"
#include "Player.hpp"

namespace zappy::gui::game {
class Team {
  public:
    Team(std::string name, std::size_t slot, raylib::Color teamColor)
        : _name(std::move(name)), _teamColor(teamColor), _eggs(slot) {};
    ~Team() = default;
    Team(const Team& other) = delete;
    Team& operator=(const Team& other) = delete;
    Team(Team&& other) noexcept = default;
    Team& operator=(Team&& other) noexcept = default;

    void draw(const GameModel& gameModel) const;

    Player& addPlayer(int id, raylib::rmath::Vector3 position, Player::cardinalPoint orientation,
                      std::size_t level = 1);

    void updatePlayerName(int id, const std::string& name);

    [[nodiscard]] bool hasPlayer(int id) const;
    [[nodiscard]] std::optional<std::reference_wrapper<Player>> findPlayer(int id);
    [[nodiscard]] std::optional<std::reference_wrapper<const Player>> findPlayer(int id) const;
    void removePlayer(int id);
    void addEgg(int id, int playerId, const raylib::rmath::Vector3& position);
    void removeEgg(int id);
    [[nodiscard]] const std::string& name() const { return _name; }
    [[nodiscard]] const std::vector<Player>& players() const { return _players; }
    [[nodiscard]] const std::vector<Egg>& eggs() const { return _eggs; }
    [[nodiscard]] raylib::Color color() const { return _teamColor; }

  protected:
  private:
    std::string _name;
    raylib::Color _teamColor;
    std::vector<Player> _players;
    std::vector<Egg> _eggs;
};
}  // namespace zappy::gui::game
