/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Command facotry for gui commands test
*/

#include <gtest/gtest.h>

#include <string>

#include "command/CommandFactory.hpp"
#include "guiCommand/Sbp.hpp"

TEST(GuiCommandFactoryTest, CreateMszCommand) {
    const zappy::server::command::CommandFactory factory;
    const std::string rawCommand = "msz";
    auto command = factory.createGuiCommand(rawCommand);

    EXPECT_NE(command, nullptr);
}
TEST(GuiCommandFactoryTest, CreateInvalidCommandReturnsNullptr) {
    const zappy::server::command::CommandFactory factory;
    const std::string rawCommand = "invalid_command";
    auto command = factory.createGuiCommand(rawCommand);

    EXPECT_EQ(command, nullptr);
}

TEST(GuiCommandFactoryTest, CreateBctCommand) {
    const zappy::server::command::CommandFactory factory;
    const std::string rawCommand = "bct 1 2";
    auto command = factory.createGuiCommand(rawCommand);

    EXPECT_NE(command, nullptr);
}

TEST(GuiCommandFactoryTest, CreateBctCommandWithInvalidParamsReturnsNullptr) {
    const zappy::server::command::CommandFactory factory;
    const std::string rawCommand = "bct invalid_params";
    auto command = factory.createGuiCommand(rawCommand);

    ASSERT_NE(command, nullptr);
    const auto& sbpCommand = dynamic_cast<zappy::server::guiCommand::Sbp*>(command.get());
    EXPECT_NE(sbpCommand, nullptr);
}

TEST(GuiCommandFactoryTest, CreateBctCommandWithMissingParamsReturnsNullptr) {
    const zappy::server::command::CommandFactory factory;
    const std::string rawCommand = "bct 1";
    auto command = factory.createGuiCommand(rawCommand);

    ASSERT_NE(command, nullptr);
    const auto& sbpCommand = dynamic_cast<zappy::server::guiCommand::Sbp*>(command.get());
    EXPECT_NE(sbpCommand, nullptr);
}

TEST(GuiCommandFactoryTest, CreateBctCommandWithExtraParamsReturnsNullptr) {
    const zappy::server::command::CommandFactory factory;
    const std::string rawCommand = "bct 1 2 extra_param";
    auto command = factory.createGuiCommand(rawCommand);

    ASSERT_NE(command, nullptr);
    const auto& sbpCommand = dynamic_cast<zappy::server::guiCommand::Sbp*>(command.get());
    EXPECT_NE(sbpCommand, nullptr);
}

TEST(GuiCommandFactoryTest, CreateMctCommand) {
    const zappy::server::command::CommandFactory factory;
    const std::string rawCommand = "mct";
    auto command = factory.createGuiCommand(rawCommand);

    EXPECT_NE(command, nullptr);
}

TEST(GuiCommandFactoryTest, CreateTnaCommand) {
    const zappy::server::command::CommandFactory factory;
    const std::string rawCommand = "tna";
    auto command = factory.createGuiCommand(rawCommand);

    EXPECT_NE(command, nullptr);
}

TEST(GuiCommandFactoryTest, CreateSstCommand) {
    const zappy::server::command::CommandFactory factory;
    std::string rawCommand = "sst 10   ";
    auto command = factory.createGuiCommand(rawCommand);

    EXPECT_NE(command, nullptr);
    rawCommand = "sst invalid_param";
    command = factory.createGuiCommand(rawCommand);
    ASSERT_NE(command, nullptr);
    const auto& sbpCommand = dynamic_cast<zappy::server::guiCommand::Sbp*>(command.get());
    EXPECT_NE(sbpCommand, nullptr);
}

TEST(GuiCommandFactoryTest, CreateSgtCommand) {
    const zappy::server::command::CommandFactory factory;
    std::string rawCommand = "sgt 10 ";
    auto command = factory.createGuiCommand(rawCommand);

    EXPECT_NE(command, nullptr);
    rawCommand = "sgt invalid_param";
    command = factory.createGuiCommand(rawCommand);
    ASSERT_NE(command, nullptr);
    const auto& sbpCommand = dynamic_cast<zappy::server::guiCommand::Sbp*>(command.get());
    EXPECT_NE(sbpCommand, nullptr);
}
