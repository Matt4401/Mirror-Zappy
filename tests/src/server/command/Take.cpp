/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** take command tests
*/

#include "command/Take.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>

#include "command/ICommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server::command {

TEST(TakeTest, CheckRequiredTicks) {
    const std::unique_ptr<ICommand> take = std::make_unique<Take>("food");
    const auto ticks = take->requiredTicks();
    ASSERT_EQ(ticks, 7);
}

TEST(TakeTest, CheckStartInvalidItem) {
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

    Take take{"wrongItem"};

    ASSERT_FALSE(take.start(world, player));
}

TEST(TakeTest, CheckStartValidItemButEmptyTile) {
    const auto config = parser::ServerConfig{
        .port = 80,
        .width = 16,
        .height = 16,
        .teamNames = {"test"},
        .clientLimit = 1,
        .freq = 100,
    };
    game::World world{config};

    auto playerIdOpt = world.spawnPlayer("test");
    ASSERT_TRUE(playerIdOpt.has_value());

    if (!playerIdOpt.has_value()) {
        return;
    }

    auto& player = *world.playerList().at(playerIdOpt.value());

    constexpr auto targetPos = game::Position{.x = 5, .y = 5};
    world.updatePositionOnMap(player.id(), player.position(), targetPos);
    player.setPosition(targetPos);

    world.clearAllResourcesAndEggs();

    Take take{"food"};

    ASSERT_FALSE(take.start(world, player));
}

TEST(TakeTest, CheckStartSuccess) {
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

    world.addItemOnGround(game::ItemType::Food, player.position());

    Take take{"food"};

    ASSERT_TRUE(take.start(world, player));
}

TEST(TakeTest, CheckExecuteMovementAndInventory) {
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

    auto nbActualFood = world.resourcesAt(player.position()).at(static_cast<std::uint8_t>(game::ItemType::Food));
    if (nbActualFood == 0) {
        world.addItemOnGround(game::ItemType::Food, player.position());
        nbActualFood++;
    }
    while (nbActualFood != 1) {
        world.removeItemOnGround(game::ItemType::Food, player.position());
        nbActualFood--;
    }
    Take take{"food"};

    take.execute(world, player);

    const auto& resources = world.resourcesAt(player.position());

    ASSERT_EQ(resources.at(static_cast<std::uint8_t>(game::ItemType::Food)), 0);
    ASSERT_EQ(player.inventory().at(static_cast<std::uint8_t>(game::ItemType::Food)), 11);
}

TEST(TakeTest, FailedExecute) {
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

    world.clearAllResourcesAndEggs();
    Take take{"deraumere"};
    take.execute(world, player);
    ASSERT_EQ(player.responses().front(), "ko\n");
    ASSERT_EQ(player.inventory().at(static_cast<std::uint8_t>(game::ItemType::Deraumere)), 0);
}

}  // namespace zappy::server::command
