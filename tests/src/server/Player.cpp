#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {
    class [[maybe_unused]] MockCommand : public ICommand {
    public:
        MOCK_METHOD(bool, start, (game::World& world, game::Player& player), (override));
        MOCK_METHOD(void, execute, (game::World& world, game::Player& player), (override));
        MOCK_METHOD(std::size_t, requiredTicks, (), (const));
    };
}

using namespace zappy::server::game;
using testing::Return;
using testing::StrictMock;

class PlayerTest : public testing::Test {
protected:
    void SetUp() override {
        player = std::make_unique<Player>(1, 5, 5);
    }

    std::unique_ptr<Player> player;
};

TEST_F(PlayerTest, InitialState) {
    EXPECT_EQ(player->getItem(ItemType::Food), 10);
    EXPECT_EQ(player->getItem(ItemType::Linemate), 0);
    EXPECT_EQ(player->getItem(ItemType::Thystame), 0);
}

TEST_F(PlayerTest, AddAndSubItems) {
    player->addItem(ItemType::Linemate, 5);
    EXPECT_EQ(player->getItem(ItemType::Linemate), 5);

    player->subItem(ItemType::Linemate, 2);
    EXPECT_EQ(player->getItem(ItemType::Linemate), 3);
}

TEST_F(PlayerTest, SetItemDirectly) {
    player->setItem(ItemType::Deraumere, 42);
    EXPECT_EQ(player->getItem(ItemType::Deraumere), 42);
}

TEST_F(PlayerTest, ResponsesHandling) {
    player->addResponse("message 1\n");
    player->addResponse("message 2\n");

    const auto responses = player->getResponses();
    ASSERT_EQ(responses.size(), 2);
    EXPECT_EQ(responses[0], "message 1\n");
    EXPECT_EQ(responses[1], "message 2\n");
    const auto emptyResponses = player->getResponses();
    EXPECT_TRUE(emptyResponses.empty());
}


TEST_F(PlayerTest, MoveUpNorth) {
    const std::pair<std::size_t, std::size_t> limit = {9, 9};

    player->moveUp(limit);
}