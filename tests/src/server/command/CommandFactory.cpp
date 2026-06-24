/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** command factory test
*/

#include "command/CommandFactory.hpp"

#include <gtest/gtest.h>

#include <memory>

#include "command/Broadcast.hpp"
#include "command/ConnectNbr.hpp"
#include "command/Forward.hpp"
#include "guiCommand/IGuiCommand.hpp"

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

TEST(CommandFactoryTest, CreateCommandBroadcastReturnsValidCommand) {
    const CommandFactory factory;
    auto command = factory.createCommand("Broadcast Hello World");
    EXPECT_NE(command, nullptr);
    EXPECT_TRUE(dynamic_cast<Broadcast*>(command.get()) != nullptr);
}

TEST(CommandFactoryTest, CreateCommandBroadcastIgnoresExtraArguments) {
    const CommandFactory factory;
    auto command = factory.createCommand("Broadcast Hello World\n");
    EXPECT_NE(command, nullptr);
    EXPECT_TRUE(dynamic_cast<Broadcast*>(command.get()) != nullptr);
}

TEST(CommandFactoryTest, CreateCommandConnectNbrReturnsValidCommand) {
    const CommandFactory factory;
    auto command = factory.createCommand("Connect_nbr");
    EXPECT_NE(command, nullptr);
    EXPECT_TRUE(dynamic_cast<ConnectNbr*>(command.get()) != nullptr);
}

TEST(CommandFactoryTest, CreateGuiCommandReturnsNullptrForEmptyString) {
    const zappy::server::command::CommandFactory factory{};

    const std::unique_ptr<zappy::server::guiCommand::IGuiCommand> command = factory.createGuiCommand("");

    EXPECT_EQ(command, nullptr);
}

TEST(CommandFactoryTest, CreateGuiCommandReturnsMszCommand) {
    const zappy::server::command::CommandFactory factory{};

    const std::unique_ptr<zappy::server::guiCommand::IGuiCommand> command = factory.createGuiCommand("msz");

    EXPECT_NE(command, nullptr);
}

TEST(CommandFactoryTest, CreateGuiCommandReturnsNullptrForUnknownCommand) {
    const zappy::server::command::CommandFactory factory{};

    const std::unique_ptr<zappy::server::guiCommand::IGuiCommand> command =
        factory.createGuiCommand("invalid_gui_string");

    EXPECT_EQ(command, nullptr);
}

TEST(CommandFactoryTest, CreateGuiCommandReturnsNullptrForUnknownCommandWithArgs) {
    const zappy::server::command::CommandFactory factory{};

    const std::unique_ptr<zappy::server::guiCommand::IGuiCommand> command =
        factory.createGuiCommand("invalid_gui_string arg1 arg2");

    EXPECT_EQ(command, nullptr);
}

}  // namespace zappy::server::command
