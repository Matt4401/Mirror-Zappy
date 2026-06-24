/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** tests for command nbr
*/

#include "command/ConnectNbr.hpp"

#include <gtest/gtest.h>

#include <memory>

#include "command/ICommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server::command {

// NOLINTBEGIN
static const parser::ServerConfig kDefaultConfig = {
    .port = 8080,
    .width = 10,
    .height = 10,
    .teamNames = {"Team1", "Team2"},
    .clientLimit = 2,
    .freq = 100,
};

TEST(ConnectNbrTest, CheckRequiredTicks) {
    const std::unique_ptr<ICommand> command = std::make_unique<ConnectNbr>();

    ASSERT_EQ(command->requiredTicks(), 0);
}

TEST(ConnectNbrTest, StartReturnsRemainingSlots) {
    game::World world{kDefaultConfig};

    const auto idOpt = world.spawnPlayer("Team1");
    ASSERT_TRUE(idOpt.has_value());

    const auto& playerList = world.playerList();
    auto& player = *playerList.at(idOpt.value());

    const std::unique_ptr<ICommand> command = std::make_unique<ConnectNbr>();

    ASSERT_TRUE(command->start(world, player));

    const auto responses = player.responses();
    ASSERT_EQ(responses.size(), 1);
    EXPECT_EQ(responses.front(), "1\n");
}

TEST(ConnectNbrTest, StartReturnsZeroWhenTeamIsFull) {
    game::World world{kDefaultConfig};

    const auto firstIdOpt = world.spawnPlayer("Team1");
    const auto secondIdOpt = world.spawnPlayer("Team1");
    ASSERT_TRUE(firstIdOpt.has_value());
    ASSERT_TRUE(secondIdOpt.has_value());

    const auto& playerList = world.playerList();
    auto& player = *playerList.at(firstIdOpt.value());

    const std::unique_ptr<ICommand> command = std::make_unique<ConnectNbr>();

    ASSERT_TRUE(command->start(world, player));

    const auto responses = player.responses();
    ASSERT_EQ(responses.size(), 1);
    EXPECT_EQ(responses.front(), "0\n");
}

// NOLINTEND

}  // namespace zappy::server::command
