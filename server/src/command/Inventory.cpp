/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Inventory
*/

#include "Inventory.hpp"

#include <string>

#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {
Inventory::Inventory() : ACommand(kTimeLimit) {}

void Inventory::execute(game::World& /*world*/, game::Player& player) {
    auto it = game::kMapItemString.begin();
    std::string response = "[";
    while (it != game::kMapItemString.end()) {
        const std::string nbItem = std::to_string(player.getItem(it->second));
        response.append(it->first + " " + nbItem);
        ++it;
        if (it != game::kMapItemString.end()) {
            response.append(", ");
        }
    }
    response.append("]\n");
    player.addResponse(response);
}
}  // namespace zappy::server::command
