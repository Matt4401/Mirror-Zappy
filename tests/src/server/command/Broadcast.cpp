/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Broadcast tests
*/

#include "command/Broadcast.hpp"

#include <gtest/gtest.h>

#include <string>

#include "game/Player.hpp"
#include "game/World.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server::command {

namespace {

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void checkBroadcastMessage(const game::Position& receiverPos, const std::string& expectedMessage) {
    const parser::ServerConfig config = {
        .port = 8080,
        .width = 10,
        .height = 10,
        .teamNames = {"Team1"},
        .clientLimit = 2,
        .freq = 100,
    };

    game::World world{config};

    const auto senderIdOpt = world.spawnPlayer("Team1");
    const auto receiverIdOpt = world.spawnPlayer("Team1");

    ASSERT_TRUE(senderIdOpt.has_value());
    ASSERT_TRUE(receiverIdOpt.has_value());

    unsigned long senderId = 0;
    unsigned long receiverId = 0;
    if (senderIdOpt.has_value() && receiverIdOpt.has_value()) {
        senderId = senderIdOpt.value();
        receiverId = receiverIdOpt.value();
    }

    const auto& playerList = world.playerList();
    auto& sender = *playerList.at(senderId);
    auto& receiver = *playerList.at(receiverId);

    const auto oldSenderPos = sender.position();
    const auto oldReceiverPos = receiver.position();

    sender.setPosition(game::Position{.x = 5, .y = 5});
    receiver.setPosition(receiverPos);

    world.updatePositionOnMap(senderId, oldSenderPos, game::Position{.x = 5, .y = 5});
    world.updatePositionOnMap(receiverId, oldReceiverPos, receiverPos);

    Broadcast broadcast{"hello world"};
    broadcast.execute(world, sender);

    const auto responses = world.getAllResponsesBuffer();

    ASSERT_TRUE(responses.contains(senderId));
    ASSERT_TRUE(responses.contains(receiverId));
    ASSERT_FALSE(responses.at(senderId).empty());
    EXPECT_EQ(responses.at(senderId).front(), "ok\n");
    ASSERT_FALSE(responses.at(receiverId).empty());
    EXPECT_EQ(responses.at(receiverId).front(), expectedMessage);
}

}  // namespace

TEST(BroadcastTest, CheckRequiredTicks) {
    const Broadcast broadcast{"hello"};
    ASSERT_EQ(broadcast.requiredTicks(), 7);
}

TEST(BroadcastTest, ExecuteNorth) { checkBroadcastMessage({.x = 5, .y = 4}, "message 1, hello world\n"); }

TEST(BroadcastTest, ExecuteNorthWest) { checkBroadcastMessage({.x = 4, .y = 4}, "message 2, hello world\n"); }

TEST(BroadcastTest, ExecuteWest) { checkBroadcastMessage({.x = 4, .y = 5}, "message 3, hello world\n"); }

TEST(BroadcastTest, ExecuteSouthWest) { checkBroadcastMessage({.x = 4, .y = 6}, "message 4, hello world\n"); }

TEST(BroadcastTest, ExecuteSouth) { checkBroadcastMessage({.x = 5, .y = 6}, "message 5, hello world\n"); }

TEST(BroadcastTest, ExecuteSouthEast) { checkBroadcastMessage({.x = 6, .y = 6}, "message 6, hello world\n"); }

TEST(BroadcastTest, ExecuteEast) { checkBroadcastMessage({.x = 6, .y = 5}, "message 7, hello world\n"); }

TEST(BroadcastTest, ExecuteNorthEast) { checkBroadcastMessage({.x = 6, .y = 4}, "message 8, hello world\n"); }

TEST(BroadcastTest, ExecuteSame) { checkBroadcastMessage({.x = 5, .y = 5}, "message 0, hello world\n"); }

}  // namespace zappy::server::command
