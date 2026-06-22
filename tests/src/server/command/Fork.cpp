/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Fork
*/

#include "command/Fork.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "command/ICommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server::game::test {

// NOLINTBEGIN
TEST(ForkTest, CheckRequiredTicks) {
    const command::Fork forkCmd{};
    ASSERT_EQ(forkCmd.requiredTicks(), 42);
}

TEST(ForkTest, CheckStartSuccess) {
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

    command::Fork forkCmd{};

    ASSERT_TRUE(forkCmd.start(world, player));
}

TEST(ForkTest, CheckExecuteLaysEggAndRepliesOk) {
    const auto config = parser::ServerConfig{
        .port = 80,
        .width = 16,
        .height = 16,
        .teamNames = {"test"},
        .clientLimit = 1,
        .freq = 100,
    };
    game::World world{config};

    const auto playerId = world.spawnPlayer("test");
    ASSERT_TRUE(playerId.has_value());

    auto& player = *world.playerList().at(playerId.value());

    ASSERT_FALSE(world.isEggOnTile(player.position()));

    command::Fork forkCmd{};
    forkCmd.execute(world, player);

    ASSERT_TRUE(world.isEggOnTile(player.position()));

    const auto responses = world.getAllResponsesBuffer();
    ASSERT_TRUE(responses.contains(player.id()));
    ASSERT_EQ(responses.at(player.id()).front(), "ok\n");
}

TEST(ForkTest, CheckExecuteIncreasesTeamSlots) {
    const auto config = parser::ServerConfig{
        .port = 80,
        .width = 16,
        .height = 16,
        .teamNames = {"test"},
        .clientLimit = 1,
        .freq = 100,
    };
    game::World world{config};

    const auto playerId = world.spawnPlayer("test");
    ASSERT_TRUE(playerId.has_value());

    ASSERT_EQ(world.getAvailableSlotInTeam("test"), 0);

    auto& player = *world.playerList().at(playerId.value());

    command::Fork forkCmd{};
    forkCmd.execute(world, player);

    ASSERT_EQ(world.getAvailableSlotInTeam("test"), 1);
}

TEST(ForkTest, CheckExecuteGeneratesGuiEvents) {
    const auto config = parser::ServerConfig{
        .port = 80,
        .width = 16,
        .height = 16,
        .teamNames = {"test"},
        .clientLimit = 1,
        .freq = 100,
    };
    game::World world{config};
    const auto playerId = world.spawnPlayer("test");
    ASSERT_TRUE(playerId.has_value());
    auto& player = *world.playerList().at(playerId.value());

    auto events = world.getAndClearGuiEvents();

    command::Fork forkCmd{};
    forkCmd.execute(world, player);

    events = world.getAndClearGuiEvents();
    bool foundEggNew = false;

    for (const auto& event : events) {
        if (event.starts_with("enw ")) {
            foundEggNew = true;
            break;
        }
    }

    ASSERT_TRUE(foundEggNew);
}

TEST(ForkTest, CheckExecuteGeneratesPfkEvent) {
    const auto config = parser::ServerConfig{
        .port = 80,
        .width = 16,
        .height = 16,
        .teamNames = {"test"},
        .clientLimit = 1,
        .freq = 100,
    };
    game::World world{config};
    const auto playerId = world.spawnPlayer("test");
    ASSERT_TRUE(playerId.has_value());
    auto& player = *world.playerList().at(playerId.value());

    auto events = world.getAndClearGuiEvents();

    command::Fork forkCmd{};
    forkCmd.start(world, player);
    forkCmd.execute(world, player);

    events = world.getAndClearGuiEvents();
    bool foundPfkEvent = false;

    for (const auto& event : events) {
        if (event.starts_with("pfk ")) {
            foundPfkEvent = true;
            break;
        }
    }

    ASSERT_TRUE(foundPfkEvent);
}

TEST(ForkTest, CheckExecuteGeneratesPfkEventWithCorrectPlayerId) {
    const auto config = parser::ServerConfig{
        .port = 80,
        .width = 16,
        .height = 16,
        .teamNames = {"test"},
        .clientLimit = 1,
        .freq = 100,
    };
    game::World world{config};
    const auto playerId = world.spawnPlayer("test");
    ASSERT_TRUE(playerId.has_value());
    auto& player = *world.playerList().at(playerId.value());

    auto events = world.getAndClearGuiEvents();

    command::Fork forkCmd{};
    forkCmd.start(world, player);
    forkCmd.execute(world, player);

    events = world.getAndClearGuiEvents();
    bool foundPfkEventWithCorrectId = false;

    for (const auto& event : events) {
        if (event.starts_with("pfk ")) {
            std::istringstream iss(event);
            std::string prefix;
            int eventPlayerId;
            iss >> prefix >> eventPlayerId;

            if (eventPlayerId == static_cast<int>(player.id())) {
                foundPfkEventWithCorrectId = true;
                break;
            }
        }
    }

    ASSERT_TRUE(foundPfkEventWithCorrectId);
}

// NOLINTEND

}  // namespace zappy::server::game::test
