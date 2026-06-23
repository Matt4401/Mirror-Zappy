/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Team
*/

#pragma once
#include <algorithm>
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
                      std::size_t level = 1) {
        if (const auto existing = findPlayer(id); existing.has_value()) {
            return existing->get();
        }
        return _players.emplace_back(id, position, _name + std::to_string(id), orientation, level);
    }

    void updatePlayerName(int id, const std::string& name) {
        for (auto& player : _players) {
            if (player.id() == id) {
                player.setName(name);
                break;
            }
        }
    }

    [[nodiscard]] bool hasPlayer(int id) const {
        return std::ranges::any_of(_players, [id](const Player& player) { return player.id() == id; });
    }
    [[nodiscard]] std::optional<std::reference_wrapper<Player>> findPlayer(int id) {
        const auto player =
            std::ranges::find_if(_players, [id](const Player& candidate) { return candidate.id() == id; });
        if (player == _players.end()) {
            return std::nullopt;
        }
        return std::ref(*player);
    }
    [[nodiscard]] std::optional<std::reference_wrapper<const Player>> findPlayer(int id) const {
        const auto player =
            std::ranges::find_if(_players, [id](const Player& candidate) { return candidate.id() == id; });
        if (player == _players.end()) {
            return std::nullopt;
        }
        return std::cref(*player);
    }
    void removePlayer(int id) {
        std::erase_if(_players, [id](const Player& player) { return player.id() == id; });
    }
    void addEgg(int id, int playerId, const raylib::rmath::Vector3& position) {
        if (std::ranges::none_of(_eggs, [id](const Egg& egg) { return egg.id() == id; })) {
            _eggs.emplace_back(id, playerId, position);
        }
    }
    void removeEgg(int id) {
        std::erase_if(_eggs, [id](const Egg& egg) { return egg.id() == id; });
    }
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
