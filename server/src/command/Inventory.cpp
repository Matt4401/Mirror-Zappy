/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Inventory
*/

#include "Inventory.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

#include "Logger.hpp"
#include "command/ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {
Inventory::Inventory() : ACommand(kTimeLimit) {}

void Inventory::execute(game::World& /*world*/, game::Player& player) {
    std::string response = "[";

    for (std::size_t i = 0; i < game::kInventoryOrder.size(); ++i) {
        const std::string& itemName = game::kInventoryOrder.at(i);
        const game::ItemType itemEnum = game::mapItemString().at(itemName);
        const auto quantity = player.inventory().at(static_cast<std::uint8_t>(itemEnum));

        response += itemName + " " + std::to_string(quantity);
        if (i < game::kInventoryOrder.size() - 1) {
            response += ", ";
        }
    }
    response += "]\n";
    player.addResponse(response);
    Logger::logInfo("Inventory command executed for player " + std::to_string(player.id()) +
                    " with response: " + response);
}
}  // namespace zappy::server::command
