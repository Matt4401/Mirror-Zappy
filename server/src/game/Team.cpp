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
    std::ranges::any_of(_listPlayerId, [id](const auto& playerId) { return playerId == id; });
    return false;
}

void Team::removeFromTeam(const std::size_t id) {
    auto it = _listPlayerId.begin();

    for (const auto& playerId : _listPlayerId) {
        if (playerId == id) {
            _listPlayerId.erase(it);
            removeTeamSlot();
            return;
        }
        ++it;
    }
}

void Team::addInTeam(const std::size_t id) { _listPlayerId.push_back(id); }

void Team::addNewTeamSlot() { _listPlayerId.reserve(_listPlayerId.size() + 1); }

std::vector<std::size_t> Team::listPlayerId() const { return _listPlayerId; }

void Team::removeTeamSlot() {
    _nbTeamSlots--;
    _listPlayerId.shrink_to_fit();
    _listPlayerId.reserve(_nbTeamSlots - 1);
}

}  // namespace zappy::server::game
