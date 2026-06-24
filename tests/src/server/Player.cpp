/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Player tests
*/

#include "game/Player.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>

#include "command/ICommand.hpp"
#include "game/World.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server::game::tests {

namespace {

class [[maybe_unused]] MockCommand : public command::ICommand {
  public:
    MOCK_METHOD(bool, start, (game::World & world, game::Player& player), (override));
    MOCK_METHOD(void, execute, (game::World & world, game::Player& player), (override));
    MOCK_METHOD(std::size_t, requiredTicks, (), (const, override));
};

TEST(PlayerTest, InitialState) {
    const Player player{1, 5, 5, cardinalPoint::NORTH};

    EXPECT_EQ(player.getItem(ItemType::Food), 10);
    EXPECT_EQ(player.getItem(ItemType::Linemate), 0);
    EXPECT_EQ(player.getItem(ItemType::Thystame), 0);
}

TEST(PlayerTest, AddAndSubItems) {
    Player player{1, 5, 5, cardinalPoint::NORTH};

    player.addItem(ItemType::Linemate, 5);
    EXPECT_EQ(player.getItem(ItemType::Linemate), 5);

    player.subItem(ItemType::Linemate, 2);
    EXPECT_EQ(player.getItem(ItemType::Linemate), 3);
}

TEST(PlayerTest, SetItemDirectly) {
    Player player{1, 5, 5, cardinalPoint::NORTH};

    player.setItem(ItemType::Deraumere, 42);
    EXPECT_EQ(player.getItem(ItemType::Deraumere), 42);
}

TEST(PlayerTest, ResponsesHandling) {
    Player player{1, 5, 5, cardinalPoint::NORTH};

    player.addResponse("message 1\n");
    player.addResponse("message 2\n");

    const auto responses = player.responses();
    ASSERT_EQ(responses.size(), 2);
    EXPECT_EQ(responses.at(0), "message 1\n");
    EXPECT_EQ(responses.at(1), "message 2\n");

    const auto emptyResponses = player.responses();
    EXPECT_TRUE(emptyResponses.empty());
}

TEST(PlayerTest, MoveUpNorth) {
    Player player{1, 5, 5, cardinalPoint::NORTH};
    constexpr Position limit = {.x = 9, .y = 9};

    player.moveForward(limit);
}

TEST(PlayerTest, MoveUpNegativeWraparound) {
    Player player{1, 0, 0, cardinalPoint::NORTH};
    constexpr Position limit = {.x = 9, .y = 9};

    player.moveForward(limit);

    const auto [x, y] = player.position();
    EXPECT_EQ(x, 0);
    EXPECT_EQ(y, 1);
}

TEST(PlayerTest, foodDecreasedPlayer) {
    Player player{1, 0, 0, cardinalPoint::NORTH};
    World world{parser::ServerConfig{
        .port = 4242,
        .width = 10,
        .height = 10,
        .teamNames = {"team1", "team2"},
        .clientLimit = 3,
        .freq = 100,
    }};

    for (int i = 0; i < 126; ++i) {
        player.update(world);
    }
    ASSERT_EQ(player.inventory().at(static_cast<std::uint8_t>(ItemType::Food)), 9);
    const auto inventory = player.inventory();
    auto [x, y] = player.position();
    ASSERT_EQ(world.getAndClearGuiEvents().front(),
              shared::protocol::Emitter::build(shared::protocol::server::Pin{
                  .playerId = static_cast<int>(player.id()),
                  .x = static_cast<int>(x),
                  .y = static_cast<int>(y),
                  .food = static_cast<int>(inventory.at(static_cast<std::uint8_t>(ItemType::Food))),
                  .linemate = static_cast<int>(inventory.at(static_cast<std::uint8_t>(ItemType::Linemate))),
                  .deraumere = static_cast<int>(inventory.at(static_cast<std::uint8_t>(ItemType::Deraumere))),
                  .sibur = static_cast<int>(inventory.at(static_cast<std::uint8_t>(ItemType::Sibur))),
                  .mendiane = static_cast<int>(inventory.at(static_cast<std::uint8_t>(ItemType::Mendiane))),
                  .phiras = static_cast<int>(inventory.at(static_cast<std::uint8_t>(ItemType::Phiras))),
                  .thystame = static_cast<int>(inventory.at(static_cast<std::uint8_t>(ItemType::Thystame))),
              }));
}

TEST(PlayerTest, starveToDeath) {
    Player player{1, 0, 0, cardinalPoint::NORTH};
    World world{parser::ServerConfig{
        .port = 4242,
        .width = 10,
        .height = 10,
        .teamNames = {"team1", "team2"},
        .clientLimit = 3,
        .freq = 100,
    }};

    for (int i = 0; i < 126 * 10; ++i) {
        player.update(world);
    }
    ASSERT_EQ(player.inventory().at(static_cast<std::uint8_t>(ItemType::Food)), 0);
    const auto guiEvents = world.getAndClearGuiEvents();
    ASSERT_EQ(guiEvents.size(), 10);

    ASSERT_EQ(guiEvents.back(), shared::protocol::Emitter::build(shared::protocol::server::Pin{
                                    .playerId = static_cast<int>(player.id()),
                                }));
}

}  // namespace
}  // namespace zappy::server::game::tests
