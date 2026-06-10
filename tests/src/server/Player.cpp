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

#include "command/ICommand.hpp"
#include "game/World.hpp"

namespace zappy::server::game::tests {

namespace {

class [[maybe_unused]] MockCommand : public command::ICommand {
  public:
    MOCK_METHOD(bool, start, (game::World & world, game::Player& player), (override));
    MOCK_METHOD(void, execute, (game::World & world, game::Player& player), (override));
    MOCK_METHOD(std::size_t, requiredTicks, (), (const, override));
};

TEST(PlayerTest, InitialState) {
    const Player player{1, 5, 5};

    EXPECT_EQ(player.getItem(ItemType::Food), 10);
    EXPECT_EQ(player.getItem(ItemType::Linemate), 0);
    EXPECT_EQ(player.getItem(ItemType::Thystame), 0);
}

TEST(PlayerTest, AddAndSubItems) {
    Player player{1, 5, 5};

    player.addItem(ItemType::Linemate, 5);
    EXPECT_EQ(player.getItem(ItemType::Linemate), 5);

    player.subItem(ItemType::Linemate, 2);
    EXPECT_EQ(player.getItem(ItemType::Linemate), 3);
}

TEST(PlayerTest, SetItemDirectly) {
    Player player{1, 5, 5};

    player.setItem(ItemType::Deraumere, 42);
    EXPECT_EQ(player.getItem(ItemType::Deraumere), 42);
}

TEST(PlayerTest, ResponsesHandling) {
    Player player{1, 5, 5};

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
    Player player{1, 5, 5};
    constexpr pos limit = {.x = 9, .y = 9};

    player.moveUp(limit);
}

TEST(PlayerTest, MoveUpNegativeWraparound) {
    Player player{1, 0, 0};
    constexpr pos limit = {.x = 9, .y = 9};

    player.moveUp(limit);

    const auto [x, y] = player.position();
    EXPECT_EQ(x, 0);
    EXPECT_EQ(y, 1);
}

}  // namespace
}  // namespace zappy::server::game::tests
