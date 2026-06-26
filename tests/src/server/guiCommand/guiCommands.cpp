/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** guicommands tests
*/

#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <string>

#include "Core.hpp"
#include "game/Player.hpp"
#include "guiCommand/Bct.hpp"
#include "guiCommand/Mct.hpp"
#include "guiCommand/Msz.hpp"
#include "guiCommand/Pin.hpp"
#include "guiCommand/Plv.hpp"
#include "guiCommand/Ppo.hpp"
#include "guiCommand/Sgt.hpp"
#include "guiCommand/Tna.hpp"
#include "protocol/Commands.hpp"
#include "strategy/ServerStrategy.hpp"

const auto createDummyConfig = []() {
    return zappy::parser::ServerConfig{
        .port = 4242,
        .width = 10,
        .height = 20,
        .teamNames = {"teamA", "teamB"},
        .clientLimit = 5,
        .freq = 50,
    };
};

TEST(MszCommandTest, ExecuteReturnsCorrectDimensions) {
    zappy::server::Core core{createDummyConfig()};

    zappy::server::guiCommand::Msz command{};
    const std::string response = command.execute(core).message;

    EXPECT_EQ(response, "msz 10 20\n");
}

TEST(SgtCommandTest, ExecuteReturnsCorrectFrequency) {
    zappy::server::Core core{createDummyConfig()};

    zappy::server::guiCommand::Sgt command{};
    const std::string response = command.execute(core).message;

    EXPECT_EQ(response, "sgt 50\n");
}

TEST(BctCommandTest, ExecuteReturnsProperlyFormattedTileContent) {
    zappy::server::Core core{createDummyConfig()};

    zappy::server::guiCommand::Bct command{zappy::shared::protocol::client::Bct{.x = 5, .y = 5}};
    const std::string response = command.execute(core).message;

    auto resources = core.world().resourcesAt(zappy::server::game::Position{.x = 5, .y = 5});

    std::string const expectedResponse =
        "bct 5 5 " + std::to_string(resources.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Food))) +
        " " + std::to_string(resources.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Linemate))) + " " +
        std::to_string(resources.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Deraumere))) + " " +
        std::to_string(resources.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Sibur))) + " " +
        std::to_string(resources.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Mendiane))) + " " +
        std::to_string(resources.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Phiras))) + " " +
        std::to_string(resources.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Thystame))) + "\n";

    EXPECT_EQ(response, expectedResponse);
}

TEST(BctCommandTest, ExecuteFailsSafelyOnOutOfBounds) {
    zappy::server::Core core{createDummyConfig()};

    zappy::server::guiCommand::Bct command{zappy::shared::protocol::client::Bct{.x = 100, .y = 100}};
    const std::string response = command.execute(core).message;

    EXPECT_TRUE(response == "sbp\n");
}

TEST(MctCommandTest, ExecuteReturnsAllTileContents) {
    zappy::server::Core core{createDummyConfig()};

    zappy::server::guiCommand::Mct command{};
    const std::string response = command.execute(core).message;
    const auto mapSize = core.world().sizeMap();
    std::string expectedResponse;

    for (std::size_t x = 0; x < mapSize.x; ++x) {
        for (std::size_t y = 0; y < mapSize.y; ++y) {
            auto resources = core.world().resourcesAt(zappy::server::game::Position{.x = x, .y = y});
            expectedResponse +=
                "bct " + std::to_string(x) + " " + std::to_string(y) + " " +
                std::to_string(resources.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Food))) + " " +
                std::to_string(resources.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Linemate))) + " " +
                std::to_string(resources.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Deraumere))) +
                " " + std::to_string(resources.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Sibur))) +
                " " + std::to_string(resources.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Mendiane))) +
                " " + std::to_string(resources.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Phiras))) +
                " " + std::to_string(resources.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Thystame))) +
                "\n";
        }
    }
    EXPECT_EQ(response, expectedResponse);
}

TEST(TnaCommandTest, ExecuteReturnsTeamNames) {
    zappy::server::Core core{createDummyConfig()};

    zappy::server::guiCommand::Tna command{};
    const std::string response = command.execute(core).message;

    EXPECT_EQ(response, "tna teamA\ntna teamB\n");
}

TEST(PinCommandTest, PinCmd) {
    zappy::server::Core core{createDummyConfig()};

    // NOLINTNEXTLINE
    auto& world = const_cast<zappy::server::game::World&>(core.world());

    auto playerIdOpt = world.spawnPlayer("teamA");

    ASSERT_TRUE(playerIdOpt.has_value());

    if (!playerIdOpt.has_value()) {
        return;
    }

    const auto& player = world.playerList().at(playerIdOpt.value());
    const auto& inventory = player->inventory();
    zappy::server::guiCommand::Pin command{
        zappy::shared::protocol::client::Pin{.playerId = static_cast<int>(playerIdOpt.value())}};

    const std::string response = command.execute(core).message;
    auto [x, y] = player->position();
    std::string expectedResponse;
    expectedResponse +=
        "pin #" + std::to_string(playerIdOpt.value()) + " " + std::to_string(x) + " " + std::to_string(y) + " " +
        std::to_string(inventory.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Food))) + " " +
        std::to_string(inventory.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Linemate))) + " " +
        std::to_string(inventory.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Deraumere))) + " " +
        std::to_string(inventory.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Sibur))) + " " +
        std::to_string(inventory.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Mendiane))) + " " +
        std::to_string(inventory.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Phiras))) + " " +
        std::to_string(inventory.at(static_cast<std::uint8_t>(zappy::server::game::ItemType::Thystame))) + "\n";
    ASSERT_EQ(response, expectedResponse);
}

TEST(PlvCommandTest, ExecuteReturnsProperlyFormattedTileContent) {
    zappy::server::Core core{createDummyConfig()};

    // NOLINTNEXTLINE
    auto& world = const_cast<zappy::server::game::World&>(core.world());

    auto playerIdOpt = world.spawnPlayer("teamA");

    ASSERT_TRUE(playerIdOpt.has_value());

    if (!playerIdOpt.has_value()) {
        return;
    }

    zappy::server::guiCommand::Plv command{
        zappy::shared::protocol::client::Plv{.playerId = static_cast<int>(playerIdOpt.value())}};

    const std::string response = command.execute(core).message;
    const std::string expectedResponse = "plv #" + std::to_string(playerIdOpt.value()) + " " + std::to_string(1) + "\n";
    EXPECT_EQ(response, expectedResponse);
}

TEST(PlvCommandTest, ExecuteFailsSafelyOnOutOfBounds) {
    zappy::server::Core core{createDummyConfig()};

    zappy::server::guiCommand::Plv command{zappy::shared::protocol::client::Plv{.playerId = 100}};
    const std::string response = command.execute(core).message;

    EXPECT_TRUE(response == "sbp\n");
}

TEST(PpoCommandTest, PpoCmd) {
    zappy::server::Core core{createDummyConfig()};

    // NOLINTNEXTLINE
    auto& world = const_cast<zappy::server::game::World&>(core.world());

    auto playerIdOpt = world.spawnPlayer("teamA");

    ASSERT_TRUE(playerIdOpt.has_value());

    if (!playerIdOpt.has_value()) {
        return;
    }

    const auto& player = world.playerList().at(playerIdOpt.value());
    zappy::server::guiCommand::Ppo command{
        zappy::shared::protocol::client::Ppo{.playerId = static_cast<int>(playerIdOpt.value())}};

    const std::string response = command.execute(core).message;
    auto [x, y] = player->position();
    std::string expectedResponse;
    expectedResponse += "ppo #" + std::to_string(playerIdOpt.value()) + " " + std::to_string(x) + " " +
                        std::to_string(y) + " " + std::to_string(static_cast<int>(player->orientation()) + 1) + "\n";
    ASSERT_EQ(response, expectedResponse);
}

TEST(PpoCommandTest, PpoCmdFailsOnInvalidPlayer) {
    zappy::server::Core core{createDummyConfig()};

    zappy::server::guiCommand::Ppo command{zappy::shared::protocol::client::Ppo{.playerId = 9999}};

    const std::string response = command.execute(core).message;

    ASSERT_EQ(response, "sbp\n");
}
