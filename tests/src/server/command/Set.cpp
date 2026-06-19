/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** set command tests
*/

#include "command/Set.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>

#include "command/ICommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server::command {

TEST(SetTest, CheckRequiredTicks) {
    const std::unique_ptr<ICommand> set = std::make_unique<Set>("deraumere");
    const auto ticks = set->requiredTicks();
    ASSERT_EQ(ticks, 7);
}

TEST(SetTest, CheckStartInvalidItem) {
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

    Set set{"ressource_inconnue_au_bataillon"};

    ASSERT_FALSE(set.start(world, player));
}

TEST(SetTest, CheckStartValidItemButEmptyInventory) {
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

    Set set{"deraumere"};

    ASSERT_FALSE(set.start(world, player));
}

TEST(SetTest, CheckStartSuccess) {
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

    const game::ItemType item = game::kMapItemString.at("deraumere");
    player.addItem(item);

    Set set{"deraumere"};

    ASSERT_TRUE(set.start(world, player));
}

TEST(SetTest, CheckExecuteInventoryAndWorld) {
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

    const game::ItemType item = game::kMapItemString.at("deraumere");
    player.addItem(item);

    Set set{"deraumere"};

    const auto itemIdx = static_cast<std::uint8_t>(item);
    const auto initialGroundCount = world.tileResources(player.position()).at(itemIdx);

    set.execute(world, player);

    ASSERT_EQ(player.inventory().at(itemIdx), 0);

    const auto finalGroundCount = world.tileResources(player.position()).at(itemIdx);
    ASSERT_EQ(finalGroundCount, initialGroundCount + 1);
}

}  // namespace zappy::server::command
