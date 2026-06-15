/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World tests
*/

#include "game/World.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "command/Forward.hpp"
#include "game/Player.hpp"
#include "game/Team.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server::game {
// NOLINTBEGIN
class WorldTest : public testing::Test {
  protected:
    parser::ServerConfig config{
        .port = 4242, .width = 10, .height = 10, .teamNames = {"team1", "team2"}, .clientLimit = 5, .freq = 100};
};
// NOLINTEND

TEST_F(WorldTest, ConstructorCreatesCorrectMapSize) {
    const World world{config};
    const auto [maxX, maxY] = world.sizeMap();

    ASSERT_EQ(maxX, 9);
    ASSERT_EQ(maxY, 9);
}

TEST_F(WorldTest, ConstructorInitializesTeams) {
    World world{config};

    const auto playerId1 = world.spawnPlayer("team1");
    const auto playerId2 = world.spawnPlayer("team2");

    ASSERT_TRUE(playerId1.has_value());
    ASSERT_TRUE(playerId2.has_value());
}

TEST_F(WorldTest, ConstructorCreatesEggsForEachTeam) {
    World world{config};

    for (std::size_t i = 0; i < config.clientLimit; ++i) {
        const auto playerId = world.spawnPlayer("team1");
        ASSERT_TRUE(playerId.has_value()) << "Failed to spawn player " << i;
    }

    const auto playerId = world.spawnPlayer("team1");
    ASSERT_FALSE(playerId.has_value());
}

TEST_F(WorldTest, ConstructorWithZeroSize) {
    parser::ServerConfig zeroConfig = config;
    zeroConfig.width = 0;
    zeroConfig.height = 0;

    const World world{zeroConfig};
    const auto [maxX, maxY] = world.sizeMap();

    ASSERT_EQ(maxX, 0);
    ASSERT_EQ(maxY, 0);
}

TEST_F(WorldTest, SpawnPlayerReturnsValidId) {
    World world{config};

    const auto playerId = world.spawnPlayer("team1");

    ASSERT_TRUE(playerId.has_value());
    ASSERT_GE(playerId.value(), 0);
}

TEST_F(WorldTest, SpawnPlayerWithInvalidTeam) {
    World world{config};

    const auto playerId = world.spawnPlayer("invalid_team");

    ASSERT_FALSE(playerId.has_value());
}

TEST_F(WorldTest, SpawnPlayerConsumesEgg) {
    parser::ServerConfig smallConfig = config;
    smallConfig.clientLimit = 1;

    World world{smallConfig};

    const auto playerId1 = world.spawnPlayer("team1");
    ASSERT_TRUE(playerId1.has_value());

    const auto playerId2 = world.spawnPlayer("team1");
    ASSERT_FALSE(playerId2.has_value());
}

TEST_F(WorldTest, SpawnPlayerDifferentTeams) {
    World world{config};

    const auto player1 = world.spawnPlayer("team1");
    const auto player2 = world.spawnPlayer("team2");

    ASSERT_TRUE(player1.has_value());
    ASSERT_TRUE(player2.has_value());
    ASSERT_NE(player1.value(), player2.value());
}

TEST_F(WorldTest, GetTilePositionConverts1DTo2D) {
    const World world{config};

    auto pos = world.getTilePosition(0);
    ASSERT_EQ(pos.x, 0);
    ASSERT_EQ(pos.y, 0);

    pos = world.getTilePosition(5);
    ASSERT_EQ(pos.x, 5);
    ASSERT_EQ(pos.y, 0);

    pos = world.getTilePosition(15);
    ASSERT_EQ(pos.x, 5);
    ASSERT_EQ(pos.y, 1);
}

TEST_F(WorldTest, UpdatePositionOnMapMovesPlayer) {
    World world{config};

    const auto playerId = world.spawnPlayer("team1");
    ASSERT_TRUE(playerId.has_value());

    const Position oldPos{.x = 0, .y = 0};
    const Position newPos{.x = 5, .y = 5};

    // Cette méthode ne devrait pas crash
    ASSERT_NO_THROW(world.updatePositionOnMap(playerId.value(), oldPos, newPos));
}

// ========== Player Management ==========

TEST_F(WorldTest, RemovePlayerRemovesFromWorld) {
    World world{config};

    const auto playerId = world.spawnPlayer("team1");
    ASSERT_TRUE(playerId.has_value());

    const auto removedId = world.removePlayer(playerId.value());
    ASSERT_EQ(removedId, playerId.value());
}

TEST_F(WorldTest, GetAllResponsesBufferEmpty) {
    const World world{config};

    const auto responses = world.getAllResponsesBuffer();

    ASSERT_TRUE(responses.empty());
}

TEST_F(WorldTest, UpdateDoesNotCrashWithNoPlayers) {
    World world{config};

    ASSERT_NO_THROW(world.update());
}

TEST_F(WorldTest, UpdateDoesNotCrashWithPlayers) {
    World world{config};

    [[maybe_unused]] auto p1 = world.spawnPlayer("team1");
    [[maybe_unused]] auto p2 = world.spawnPlayer("team2");

    ASSERT_NO_THROW(world.update());
}

TEST_F(WorldTest, SizeMapReturnsDimensions) {
    parser::ServerConfig customConfig = config;
    customConfig.width = 20;
    customConfig.height = 15;

    const World world{customConfig};
    const auto [maxX, maxY] = world.sizeMap();

    ASSERT_EQ(maxX, 19);
    ASSERT_EQ(maxY, 14);
}

TEST_F(WorldTest, RandomCardinalPointReturnsValidOrientation) {
    for (int i = 0; i < 100; ++i) {
        const auto orientation = World::randomCardinalPoint();

        ASSERT_GE(static_cast<uint8_t>(orientation), 0);
        ASSERT_LT(static_cast<uint8_t>(orientation), static_cast<uint8_t>(cardinalPoint::COUNT));
    }
}

TEST_F(WorldTest, CollectAndKillDeadPlayersReturnsEmptyWhenNoDeaths) {
    World world{config};

    [[maybe_unused]] auto p = world.spawnPlayer("team1");

    const auto deadPlayers = world.collectAndKillDeadPlayers();

    ASSERT_TRUE(deadPlayers.empty());
}

TEST_F(WorldTest, SpawnMultiplePlayersInSameTeam) {
    World world{config};

    std::vector<std::optional<std::size_t>> playerIds;

    for (std::size_t i = 0; i < 3; ++i) {
        playerIds.push_back(world.spawnPlayer("team1"));
        ASSERT_TRUE(playerIds.back().has_value());
    }

    for (std::size_t i = 0; i < playerIds.size(); ++i) {
        for (std::size_t j = i + 1; j < playerIds.size(); ++j) {
            ASSERT_NE(playerIds.at(i).value(), playerIds.at(j).value());
        }
    }
}

TEST_F(WorldTest, SpawnMaxPlayersAcrossTeams) {
    World world{config};
    std::size_t totalSpawned = 0;

    for (std::size_t i = 0; i < config.clientLimit; ++i) {
        if (world.spawnPlayer("team1").has_value()) {
            ++totalSpawned;
        }
    }

    for (std::size_t i = 0; i < config.clientLimit; ++i) {
        if (world.spawnPlayer("team2").has_value()) {
            ++totalSpawned;
        }
    }

    ASSERT_EQ(totalSpawned, config.clientLimit * config.teamNames.size());
}

TEST_F(WorldTest, UpdateExecutesCommandAfterRequiredTicks) {
    World world{config};

    const auto playerId = world.spawnPlayer("team1");
    ASSERT_TRUE(playerId.has_value());

    world.pushCommandToPlayer(playerId.value(), std::make_unique<command::Forward>());

    for (int i = 0; i < 7; ++i) {
        world.update();
    }

    auto responses = world.getAllResponsesBuffer();
    ASSERT_TRUE(responses.empty() || !responses.contains(playerId.value()) || responses.at(playerId.value()).empty());

    // 8th update - command should execute now
    world.update();

    responses = world.getAllResponsesBuffer();
    ASSERT_FALSE(responses.empty());
    ASSERT_EQ(responses.at(playerId.value()).size(), 1);
    ASSERT_EQ(responses.at(playerId.value()).at(0), "ok\n");
}

TEST_F(WorldTest, UpdateExecutesMultipleCommandsInSequence) {
    World world{config};

    const auto playerId = world.spawnPlayer("team1");
    ASSERT_TRUE(playerId.has_value());

    // Push 3 Forward commands
    world.pushCommandToPlayer(playerId.value(), std::make_unique<command::Forward>());
    world.pushCommandToPlayer(playerId.value(), std::make_unique<command::Forward>());
    world.pushCommandToPlayer(playerId.value(), std::make_unique<command::Forward>());

    // 1st command executes on the 8th update
    for (int i = 0; i < 8; ++i) {
        world.update();
    }

    auto responses = world.getAllResponsesBuffer();
    ASSERT_EQ(responses.at(playerId.value()).size(), 1);

    // 2nd command executes on the 8th update after the first
    for (int i = 0; i < 8; ++i) {
        world.update();
    }

    responses = world.getAllResponsesBuffer();
    ASSERT_EQ(responses.at(playerId.value()).size(), 1);

    // 3rd command executes on the 8th update after the second
    for (int i = 0; i < 8; ++i) {
        world.update();
    }

    responses = world.getAllResponsesBuffer();
    ASSERT_EQ(responses.at(playerId.value()).size(), 1);
}

TEST_F(WorldTest, UpdateDoesNotExecuteCommandBeforeRequiredTicks) {
    World world{config};

    const auto playerId = world.spawnPlayer("team1");
    ASSERT_TRUE(playerId.has_value());

    world.pushCommandToPlayer(playerId.value(), std::make_unique<command::Forward>());

    // Update only 3 times - way below the 8 required
    for (int i = 0; i < 3; ++i) {
        world.update();
    }

    const auto responses = world.getAllResponsesBuffer();
    ASSERT_TRUE(responses.empty() || responses.find(playerId.value()) == responses.end() ||
                responses.at(playerId.value()).empty());
}

TEST_F(WorldTest, MultiplePlayersUpdateIndependently) {
    World world{config};

    const auto player1 = world.spawnPlayer("team1");
    const auto player2 = world.spawnPlayer("team2");
    ASSERT_TRUE(player1.has_value());
    ASSERT_TRUE(player2.has_value());

    world.pushCommandToPlayer(player1.value(), std::make_unique<command::Forward>());
    world.pushCommandToPlayer(player2.value(), std::make_unique<command::Forward>());

    // Both commands execute on the 8th update
    for (int i = 0; i < 8; ++i) {
        world.update();
    }

    const auto responses = world.getAllResponsesBuffer();
    ASSERT_EQ(responses.size(), 2);
    ASSERT_EQ(responses.at(player1.value()).size(), 1);
    ASSERT_EQ(responses.at(player2.value()).size(), 1);
}

TEST_F(WorldTest, CommandExecutionClearsResponseBuffer) {
    World world{config};

    const auto playerId = world.spawnPlayer("team1");
    ASSERT_TRUE(playerId.has_value());

    world.pushCommandToPlayer(playerId.value(), std::make_unique<command::Forward>());

    for (int i = 0; i < 8; ++i) {
        world.update();
    }

    // First call should return the response
    auto responses1 = world.getAllResponsesBuffer();
    ASSERT_EQ(responses1.at(playerId.value()).size(), 1);

    // Second call should return empty (buffer is cleared after first call)
    auto responses2 = world.getAllResponsesBuffer();
    ASSERT_TRUE(responses2.empty() || responses2.find(playerId.value()) == responses2.end() ||
                responses2.at(playerId.value()).empty());
}

TEST_F(WorldTest, UpdateWithNoCommandsDoesNotGenerateResponses) {
    World world{config};

    const auto playerId = world.spawnPlayer("team1");
    ASSERT_TRUE(playerId.has_value());

    // Update without pushing any commands
    for (int i = 0; i < 10; ++i) {
        world.update();
    }

    const auto responses = world.getAllResponsesBuffer();
    ASSERT_TRUE(responses.empty());
}

}  // namespace zappy::server::game
