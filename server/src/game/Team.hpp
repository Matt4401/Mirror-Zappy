/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Team
*/

#pragma once
#include <cstddef>
#include <vector>

namespace zappy::server::game {
class Team {
  public:
    explicit Team(std::size_t sizeTeam);
    ~Team() = default;

    Team(const Team& other) = delete;
    Team& operator=(const Team& other) = delete;
    Team(Team&& other) = delete;
    Team& operator=(Team&& other) = delete;

    bool isInTeam(std::size_t id);
    void removeFromTeam(std::size_t id);
    void addInTeam(std::size_t id);
    void addNewTeamSlot();
    void removeTeamSlot();
    [[nodiscard]] std::vector<std::size_t> listPlayerId() const;

  private:
    std::vector<std::size_t> _listPlayerId;
    std::size_t _nbTeamSlots;
};
}  // namespace zappy::server::game
