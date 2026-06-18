/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** inventory command tests
*/
// NOLINTBEGIN

#include "command/Inventory.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include "command/ICommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server::command {

TEST(InventoryTest, CheckRequiredTicks) {
    const std::unique_ptr<ICommand> inventory = std::make_unique<Inventory>();
    const auto ticks = inventory->requiredTicks();
    ASSERT_EQ(ticks, 1);
}

TEST(InventoryTest, CheckStartAlwaysTrue) {
    const auto config = parser::ServerConfig{
        .port = 80,
        .width = 16,
        .height = 16,
        .teamNames = {"test"},
        .clientLimit = 1,
        .freq = 100,
    };
    game::World world{config};
    game::Player player{0, 5, 5, game::cardinalPoint::NORTH};

    Inventory inventory;
    ASSERT_TRUE(inventory.start(world, player));
}

TEST(InventoryTest, CheckExecuteDefaultInventoryFormat) {
    const auto config = parser::ServerConfig{
        .port = 80,
        .width = 16,
        .height = 16,
        .teamNames = {"test"},
        .clientLimit = 1,
        .freq = 100,
    };
    game::World world{config};

    game::Player player{0, 5, 5, game::cardinalPoint::NORTH};

    Inventory inventory;
    inventory.execute(world, player);

    const std::vector<std::string> playerResponses = player.responses();

    ASSERT_EQ(playerResponses.size(), 1);

    const std::string expectedResponse =
        "[deraumere 0, food 10, linemate 0, mendiane 0, phiras 0, sibur 0, thystame 0]\n";

    ASSERT_EQ(playerResponses.at(0), expectedResponse);
}

TEST(InventoryTest, CheckExecuteModifiedInventoryFormat) {
    const auto config = parser::ServerConfig{
        .port = 80,
        .width = 16,
        .height = 16,
        .teamNames = {"test"},
        .clientLimit = 1,
        .freq = 100,
    };
    game::World world{config};
    game::Player player{0, 5, 5, game::cardinalPoint::NORTH};

    player.addItem(game::ItemType::Linemate, 3);
    player.addItem(game::ItemType::Sibur, 5);
    player.addItem(game::ItemType::Thystame, 1);

    Inventory inventory;
    inventory.execute(world, player);

    const std::vector<std::string> playerResponses = player.responses();
    ASSERT_EQ(playerResponses.size(), 1);

    const std::string expectedResponse =
        "[deraumere 0, food 10, linemate 3, mendiane 0, phiras 0, sibur 5, thystame 1]\n";

    ASSERT_EQ(playerResponses.at(0), expectedResponse);
}

}  // namespace zappy::server::command

// NOLINTEND
