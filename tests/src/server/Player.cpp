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
#include <utility>

#include "command/ICommand.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

class MockCommand : public ICommand {
  public:
    MOCK_METHOD(bool, start, (game::World & world, game::Player& player), (override));
    MOCK_METHOD(void, execute, (game::World & world, game::Player& player), (override));
    MOCK_METHOD(std::size_t, requiredTicks, (), (const, override));
};

}  // namespace zappy::server::command

namespace zappy::server::game::tests {

namespace {

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

    const auto responses = player.getResponses();
    ASSERT_EQ(responses.size(), 2);
    EXPECT_EQ(responses.at(0), "message 1\n");
    EXPECT_EQ(responses.at(1), "message 2\n");

    const auto emptyResponses = player.getResponses();
    EXPECT_TRUE(emptyResponses.empty());
}

TEST(PlayerTest, MoveUpNorth) {
    Player player{1, 5, 5};
    const std::pair<std::size_t, std::size_t> limit = {9, 9};

    player.moveUp(limit);
}

}  // namespace
}  // namespace zappy::server::game::tests
