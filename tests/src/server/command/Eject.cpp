/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Eject command tests
*/

#include "command/Eject.hpp"

#include <gtest/gtest.h>

#include <memory>

#include "command/ICommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server::command {

static const parser::ServerConfig kDefaultConfig = {
    .port = 8080,
    .width = 10,
    .height = 10,
    .teamNames = {"Team1", "Team2"},
    .clientLimit = 2,
    .freq = 100,
};

TEST(EjectTest, CheckRequiredTicks) {
    const std::unique_ptr<ICommand> eject = std::make_unique<Eject>();

    ASSERT_EQ(eject->requiredTicks(), 7);
}

TEST(EjectTest, StartFailsIfTileIsEmpty) {
    game::World world{kDefaultConfig};

    auto idOpt = world.spawnPlayer("Team1");
    ASSERT_TRUE(idOpt.has_value());

    const auto& playerList = world.playerList();
    auto& player = *playerList.at(idOpt.value());

    const std::unique_ptr<ICommand> eject = std::make_unique<Eject>();

    ASSERT_TRUE(eject->start(world, player));
}

TEST(EjectTest, ExecuteEjectsOtherPlayerAndSendsResponse) {
    game::World world{kDefaultConfig};

    auto idLauncherOpt = world.spawnPlayer("Team1");
    auto idVictimOpt = world.spawnPlayer("Team1");

    ASSERT_TRUE(idLauncherOpt.has_value());
    ASSERT_TRUE(idVictimOpt.has_value());

    auto idLauncher = idLauncherOpt.value();
    auto idVictim = idVictimOpt.value();

    const auto& playerList = world.playerList();
    auto& launcher = *playerList.at(idLauncher);
    auto& victim = *playerList.at(idVictim);

    launcher.setOrientation(game::cardinalPoint::NORTH);

    launcher.setPosition(game::Position{.x = 5, .y = 5});
    victim.setPosition(game::Position{.x = 5, .y = 5});
    world.updatePositionOnMap(idLauncher, game::Position{.x = 0, .y = 0}, game::Position{.x = 5, .y = 5});
    world.updatePositionOnMap(idVictim, game::Position{.x = 0, .y = 0}, game::Position{.x = 5, .y = 5});

    const std::unique_ptr<ICommand> eject = std::make_unique<Eject>();
    eject->execute(world, launcher);

    ASSERT_EQ(launcher.position().x, 5);
    ASSERT_EQ(launcher.position().y, 5);

    ASSERT_EQ(victim.position().x, 5);
    ASSERT_EQ(victim.position().y, 6);

    auto responses = world.getAllResponsesBuffer();

    ASSERT_FALSE(responses[idLauncher].empty());
    EXPECT_EQ(responses[idLauncher].front(), "ok\n");

    ASSERT_FALSE(responses[idVictim].empty());
    EXPECT_EQ(responses[idVictim].front(), "eject: north\n");
}

TEST(EjectTest, ExecuteDestroysEggsOnTile) {
    game::World world{kDefaultConfig};

    const auto idLauncherOpt = world.spawnPlayer("Team1");
    ASSERT_TRUE(idLauncherOpt.has_value());
    const auto idLauncher = idLauncherOpt.value();

    const auto& playerList = world.playerList();
    auto& launcher = *playerList.at(idLauncher);

    launcher.setPosition(game::Position{.x = 0, .y = 0});
    world.updatePositionOnMap(idLauncher, game::Position{.x = 0, .y = 0}, game::Position{.x = 0, .y = 0});

    ASSERT_TRUE(world.isPeopleOrEggOnTile(game::Position{.x = 0, .y = 0}));

    const std::unique_ptr<ICommand> eject = std::make_unique<Eject>();
    eject->execute(world, launcher);

    EXPECT_EQ(launcher.position().x, 0);
    EXPECT_EQ(launcher.position().y, 0);

    EXPECT_TRUE(world.isPeopleOrEggOnTile(game::Position{.x = 0, .y = 0}));
    EXPECT_FALSE(world.isEggOnTile(game::Position{.x = 0, .y = 0}));
}

}  // namespace zappy::server::command
