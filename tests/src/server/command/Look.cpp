/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Look
*/
#include "command/Look.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "command/ICommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

// NOLINTBEGIN
namespace zappy::server::command {

class LookTest : public ::testing::Test {
  protected:
    parser::ServerConfig config{
        .port = 4242,
        .width = 10,
        .height = 10,
        .teamNames = {"Team1"},
        .clientLimit = 1,
        .freq = 100,
    };
};

TEST_F(LookTest, CheckRequiredTicks) {
    const std::unique_ptr<ICommand> look = std::make_unique<Look>();
    ASSERT_EQ(look->requiredTicks(), 7);
}

TEST_F(LookTest, ExecuteLookLevel1EmptyMap) {
    game::World world{config};

    const auto idOpt = world.spawnPlayer("Team1");
    ASSERT_TRUE(idOpt.has_value());

    const auto& playerList = world.playerList();
    auto& player = *playerList.at(idOpt.value());

    world.clearAllResourcesAndEggs();
    const std::unique_ptr<ICommand> look = std::make_unique<Look>();
    look->execute(world, player);

    auto responses = player.responses();
    ASSERT_FALSE(responses.empty());
    static constexpr std::string expectedStart = "[ player,,,]\n";
    ASSERT_EQ(responses.front(), expectedStart);
}

TEST_F(LookTest, ExecuteLookWhithSomeItem) {
    game::World world{config};

    const auto idOpt = world.spawnPlayer("Team1");
    ASSERT_TRUE(idOpt.has_value());

    const auto& playerList = world.playerList();
    auto& player = *playerList.at(idOpt.value());

    world.clearAllResourcesAndEggs();
    const std::unique_ptr<ICommand> look = std::make_unique<Look>();

    world.addItemOnGround(game::ItemType::Food, game::Position{.x = 5, .y = 6});
    world.addItemOnGround(game::ItemType::Linemate, game::Position{.x = 5, .y = 6});

    auto oldPos = player.position();
    player.setOrientation(game::cardinalPoint::NORTH);
    player.setPosition(game::Position{.x = 5, .y = 5});
    world.updatePositionOnMap(idOpt.value(), oldPos, game::Position{.x = 5, .y = 5});

    look->execute(world, player);

    auto responses = player.responses();
    ASSERT_FALSE(responses.empty());
    const std::string expectedStart = "[ player,, food linemate,]\n";

    ASSERT_EQ(responses.front(), expectedStart);
}

TEST_F(LookTest, ExecuteLookWithItemsAndPlayers) {
    game::World world{config};
    game::Player player{1, 5, 5, game::cardinalPoint::NORTH};

    world.addItemOnGround(game::ItemType::Food, game::Position{.x = 5, .y = 6});
    world.addItemOnGround(game::ItemType::Linemate, game::Position{.x = 5, .y = 6});

    const std::unique_ptr<ICommand> look = std::make_unique<Look>();
    look->execute(world, player);

    auto responses = player.responses();
    ASSERT_FALSE(responses.empty());

    EXPECT_NE(responses.front().find("food"), std::string::npos);
    EXPECT_NE(responses.front().find("linemate"), std::string::npos);
}

TEST_F(LookTest, ExecuteLookAtMapEdgeWrapping) {
    game::World world{config};

    game::Player player{1, 5, 9, game::cardinalPoint::NORTH};

    world.clearAllResourcesAndEggs();
    world.addItemOnGround(game::ItemType::Food, game::Position{.x = 5, .y = 0});

    const std::unique_ptr<ICommand> look = std::make_unique<Look>();

    ASSERT_NO_THROW(look->execute(world, player));

    auto responses = player.responses();
    ASSERT_FALSE(responses.empty());
    EXPECT_NE(responses.front().find("food"), std::string::npos);
}

TEST_F(LookTest, CheckPositionsOrientationEast) {
    game::World world{config};

    world.clearAllResourcesAndEggs();
    game::Player player{1, 5, 5, game::cardinalPoint::EAST};

    auto lookPositions = player.getLookPos(world.sizeMap());

    ASSERT_EQ(lookPositions.at(0).x, 5);
    ASSERT_EQ(lookPositions.at(0).y, 5);

    ASSERT_EQ(lookPositions.at(1).x, 6);
    ASSERT_EQ(lookPositions.at(1).y, 6);
}

}  // namespace zappy::server::command
// NOLINTEND
