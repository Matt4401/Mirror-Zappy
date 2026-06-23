/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** EventDispatcherTest
*/

#include <gtest/gtest.h>

#include <memory>

#include "../../../../gui/src/events/EventDispatcher.hpp"
#include "protocol/Commands.hpp"

using namespace zappy::gui::events;
using namespace zappy::shared::protocol;

TEST(EventDispatcherTest, SubscribeAndDispatch) {
    EventDispatcher dispatcher;
    int receivedWidth = 0;
    int receivedHeight = 0;

    auto token = dispatcher.subscribe<server::Msz>([&](const server::Msz& cmd) {
        receivedWidth = cmd.width;
        receivedHeight = cmd.height;
    });

    (void)token;

    ServerCommand const cmd = server::Msz{.width = 10, .height = 20};
    dispatcher.dispatch(cmd);

    EXPECT_EQ(receivedWidth, 10);
    EXPECT_EQ(receivedHeight, 20);
}

TEST(EventDispatcherTest, Unsubscribe) {
    EventDispatcher dispatcher;
    bool called = false;

    auto token = dispatcher.subscribe<server::Msz>([&](const server::Msz&) { called = true; });

    dispatcher.unsubscribe<server::Msz>(token);

    ServerCommand const cmd = server::Msz{.width = 10, .height = 20};
    dispatcher.dispatch(cmd);

    EXPECT_FALSE(called);
}

TEST(EventDispatcherTest, MultipleListenersAndDifferentCommands) {
    EventDispatcher dispatcher;
    int mszCount = 0;
    int pnwCount = 0;

    auto token1 = dispatcher.subscribe<server::Msz>([&](const server::Msz&) { mszCount++; });
    auto token2 = dispatcher.subscribe<server::Msz>([&](const server::Msz&) { mszCount++; });
    auto token3 = dispatcher.subscribe<server::Pnw>([&](const server::Pnw&) { pnwCount++; });

    (void)token1;
    (void)token2;
    (void)token3;

    ServerCommand const cmdMsz = server::Msz{.width = 10, .height = 20};
    dispatcher.dispatch(cmdMsz);

    ServerCommand const cmdPnw =
        server::Pnw{.playerId = 1, .x = 5, .y = 5, .orientation = 1, .level = 1, .teamName = "team"};
    dispatcher.dispatch(cmdPnw);

    EXPECT_EQ(mszCount, 2);
    EXPECT_EQ(pnwCount, 1);
}

TEST(EventDispatcherTest, SupportsMoveOnlyCallback) {
    EventDispatcher dispatcher;
    int receivedWidth = 0;
    auto state = std::make_unique<int>(5);

    const auto token = dispatcher.subscribe<server::Msz>(
        [state = std::move(state), &receivedWidth](const server::Msz& cmd) { receivedWidth = *state + cmd.width; });
    (void)token;

    dispatcher.dispatch(server::Msz{.width = 10, .height = 20});

    EXPECT_EQ(receivedWidth, 15);
}
