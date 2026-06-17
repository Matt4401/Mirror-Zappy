/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Team
*/

#include "Team.hpp"

#include <algorithm>
#include <cstddef>
#include <vector>

namespace zappy::server::game {
Team::Team(const std::size_t sizeTeam) : _nbTeamSlots(sizeTeam) { _listPlayerId.reserve(sizeTeam); }
bool Team::isInTeam(std::size_t id) {
    return std::ranges::any_of(_listPlayerId, [id](const auto& playerId) { return playerId == id; });
}

void Team::removeFromTeam(const std::size_t id) {
    const auto original_size = _listPlayerId.size();
    std::erase(_listPlayerId, id);
    if (_listPlayerId.size() < original_size) {
        removeTeamSlot();
    }
}

void Team::addInTeam(const std::size_t id) { _listPlayerId.emplace_back(id); }

void Team::addNewTeamSlot() {
    _nbTeamSlots++;
    _listPlayerId.reserve(_listPlayerId.size() + 1);
}

std::vector<std::size_t> Team::listPlayerId() const { return _listPlayerId; }

std::size_t Team::availableSlot() const { return _nbTeamSlots - _listPlayerId.size(); }

void Team::removeTeamSlot() { _nbTeamSlots--; }

}  // namespace zappy::server::game
