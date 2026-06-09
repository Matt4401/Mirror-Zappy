/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** command factory test
*/

#include "command/CommandFactory.hpp"

#include <gtest/gtest.h>

#include "command/Forward.hpp"

namespace zappy::server::command {

TEST(CommandFactoryTest, CreateCommandReturnsNullptrForUnknownCommand) {
    const CommandFactory factory;
    auto command = factory.createCommand("UnknownCommand arg1 arg2");
    EXPECT_EQ(command, nullptr);
}

TEST(CommandFactoryTest, CreateCommandReturnsValidCommandForKnownCommand) {
    const CommandFactory factory;
    auto command = factory.createCommand("Forward");
    EXPECT_NE(command, nullptr);
    EXPECT_TRUE(dynamic_cast<Forward*>(command.get()) != nullptr);
}

TEST(CommandFactoryTest, CreateCommandIgnoresExtraArguments) {
    const CommandFactory factory;
    auto command = factory.createCommand("Forward\n");
    EXPECT_NE(command, nullptr);
    EXPECT_TRUE(dynamic_cast<Forward*>(command.get()) != nullptr);
}

}  // namespace zappy::server::command
