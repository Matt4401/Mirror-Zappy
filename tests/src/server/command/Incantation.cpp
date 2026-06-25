/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Incantation
*/

// NOLINTBEGIN
#include "command/Incantation.hpp"

#include <gtest/gtest.h>

#include <memory>

#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

class IncantationTest : public ::testing::Test {
  protected:
    parser::ServerConfig config{
        .port = 4242,
        .width = 10,
        .height = 10,
        .teamNames = {"Team1"},
        .clientLimit = 10,
        .freq = 100,
    };
};

TEST_F(IncantationTest, CheckRequiredTicks) {
    const std::unique_ptr<ICommand> incantation = std::make_unique<Incantation>();
    ASSERT_EQ(incantation->requiredTicks(), 300);
}

TEST_F(IncantationTest, StartFailsWithoutResources) {
    game::World world{config};
    auto playerIdOpt = world.spawnPlayer("Team1");
    ASSERT_TRUE(playerIdOpt.has_value());

    world.clearAllResourcesAndEggs();
    auto& player = *world.playerList().at(playerIdOpt.value());

    const std::unique_ptr<ICommand> incantation = std::make_unique<Incantation>();

    ASSERT_FALSE(incantation->start(world, player));
}

TEST_F(IncantationTest, StartSucceedsWithResources) {
    game::World world{config};
    auto playerIdOpt = world.spawnPlayer("Team1");
    ASSERT_TRUE(playerIdOpt.has_value());

    world.clearAllResourcesAndEggs();
    auto& player = *world.playerList().at(playerIdOpt.value());

    world.addItemOnGround(game::ItemType::Linemate, player.position());

    const std::unique_ptr<ICommand> incantation = std::make_unique<Incantation>();

    ASSERT_TRUE(incantation->start(world, player));

    auto guiEvents = world.getAndClearGuiEvents();
    ASSERT_FALSE(guiEvents.empty());
    EXPECT_NE(guiEvents.back().find("pic"), std::string::npos);
}

TEST_F(IncantationTest, ExecuteSuccessAndReplies) {
    game::World world{config};
    auto playerIdOpt = world.spawnPlayer("Team1");
    ASSERT_TRUE(playerIdOpt.has_value());

    world.clearAllResourcesAndEggs();
    auto& player = *world.playerList().at(playerIdOpt.value());
    const auto playerPos = player.position();

    ASSERT_EQ(player.level(), 1);

    world.addItemOnGround(game::ItemType::Linemate, playerPos);
    ASSERT_EQ(world.resourcesAt(playerPos).at(static_cast<std::uint8_t>(game::ItemType::Linemate)), 1);

    const std::unique_ptr<ICommand> incantation = std::make_unique<Incantation>();

    ASSERT_TRUE(incantation->start(world, player));

    incantation->execute(world, player);

    auto responses = player.responses();
    ASSERT_FALSE(responses.empty());
    ASSERT_EQ(responses.size(), 2);
    EXPECT_EQ(responses.front(), "Elevation underway\n");
    EXPECT_EQ(responses.back(), "Current level: 2\n");

    auto guiEvents = world.getAndClearGuiEvents();
    ASSERT_FALSE(guiEvents.empty());
    EXPECT_NE(guiEvents.back().find("pie"), std::string::npos);

    EXPECT_EQ(player.level(), 2);

    auto resourcesAfter = world.resourcesAt(playerPos);
    EXPECT_EQ(resourcesAfter.at(static_cast<std::uint8_t>(game::ItemType::Linemate)), 0);
}

TEST_F(IncantationTest, MultiPlayerRequirementLevel2To3) {
    game::World world{config};
    const auto id1Opt = world.spawnPlayer("Team1");
    const auto id2Opt = world.spawnPlayer("Team1");
    ASSERT_TRUE(id1Opt.has_value() && id2Opt.has_value());

    world.clearAllResourcesAndEggs();

    auto& p1 = *world.playerList().at(id1Opt.value());
    auto& p2 = *world.playerList().at(id2Opt.value());

    p1.levelUp();
    p2.levelUp();
    ASSERT_EQ(p1.level(), 2);
    ASSERT_EQ(p2.level(), 2);

    game::Position sharedPos = {.x = 5, .y = 5};
    world.updatePositionOnMap(p1.id(), p1.position(), sharedPos);
    p1.setPosition(sharedPos);

    world.updatePositionOnMap(p2.id(), p2.position(), sharedPos);
    p2.setPosition(sharedPos);

    world.addItemOnGround(game::ItemType::Linemate, sharedPos, 1);
    world.addItemOnGround(game::ItemType::Deraumere, sharedPos, 1);
    world.addItemOnGround(game::ItemType::Sibur, sharedPos, 1);

    const std::unique_ptr<ICommand> incantation = std::make_unique<Incantation>();

    ASSERT_TRUE(incantation->start(world, p1));

    incantation->execute(world, p1);

    EXPECT_EQ(p1.level(), 3);
    EXPECT_EQ(p2.level(), 3);

    auto p1Responses = p1.responses();
    ASSERT_FALSE(p1Responses.empty());
    ASSERT_EQ(p1Responses.size(), 2);
    EXPECT_EQ(p1Responses.front(), "Elevation underway\n");
    EXPECT_EQ(p1Responses.back(), "Current level: 3\n");
    auto p2Responses = p2.responses();
    ASSERT_FALSE(p2Responses.empty());
    ASSERT_EQ(p2Responses.size(), 2);
    EXPECT_EQ(p2Responses.front(), "Elevation underway\n");
    EXPECT_EQ(p2Responses.back(), "Current level: 3\n");

    auto resourcesAfter = world.resourcesAt(sharedPos);
    EXPECT_EQ(resourcesAfter.at(static_cast<std::uint8_t>(game::ItemType::Linemate)), 0);
    EXPECT_EQ(resourcesAfter.at(static_cast<std::uint8_t>(game::ItemType::Deraumere)), 0);
    EXPECT_EQ(resourcesAfter.at(static_cast<std::uint8_t>(game::ItemType::Sibur)), 0);
}

}  // namespace zappy::server::command
// NOLINTEND
