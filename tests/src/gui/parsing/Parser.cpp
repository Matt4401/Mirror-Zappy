/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GuiParserTests
*/

#include "parsing/Parser.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include "exception/ParsingError.hpp"
#include "parsing/strategy/GUIStrategy.hpp"

namespace {

std::vector<char*> buildArgv(std::vector<std::string>& args) {
    std::vector<char*> argv;

    argv.reserve(args.size());
    for (std::string& arg : args) {
        argv.push_back(arg.data());
    }
    return argv;
}

}  // namespace

namespace zappy::gui::tests {

TEST(GuiParsingTest, ParserReturnsExpectedConfigWhenArgumentsAreValid) {
    std::vector<std::string> args{"./zappy_gui", "-p", "4242", "-h", "192.168.1.50"};
    auto argv = buildArgv(args);

    parser::parsing::Parser<zappy::parser::parsing::GuiConfig> parser(std::make_unique<parser::parsing::GUIStrategy>());

    const auto [port, machine] = parser.parse(static_cast<int>(argv.size()), argv.data());

    EXPECT_EQ(port, 4242);
    EXPECT_EQ(machine, "192.168.1.50");
}

TEST(GuiParsingTest, ParserAppliesDefaultMachineWhenFlagIsMissing) {
    std::vector<std::string> args{"./zappy_gui", "-p", "8080"};
    auto argv = buildArgv(args);

    parser::parsing::Parser<zappy::parser::parsing::GuiConfig> parser(std::make_unique<parser::parsing::GUIStrategy>());

    const auto [port, machine] = parser.parse(static_cast<int>(argv.size()), argv.data());

    EXPECT_EQ(port, 8080);
    EXPECT_EQ(machine, "127.0.0.1");
}

TEST(GuiParsingTest, ParserRejectsMissingPort) {
    std::vector<std::string> args{"./zappy_gui", "-h", "127.0.0.1"};
    auto argv = buildArgv(args);

    parser::parsing::Parser<zappy::parser::parsing::GuiConfig> parser(std::make_unique<parser::parsing::GUIStrategy>());

    EXPECT_THROW(
        { [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
        shared::exception::ParsingError);
}

TEST(GuiParsingTest, ParserRejectsInvalidPorts) {
    std::vector<std::string> args{"./zappy_gui", "-p", "-5", "-h", "localhost"};
    auto argv = buildArgv(args);

    parser::parsing::Parser<zappy::parser::parsing::GuiConfig> parser(std::make_unique<parser::parsing::GUIStrategy>());

    EXPECT_THROW(
        { [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
        shared::exception::ParsingError);
}

TEST(GuiParsingTest, ParserRejectsNonNumericPort) {
    std::vector<std::string> args{"./zappy_gui", "-p", "not_a_number"};
    auto argv = buildArgv(args);

    parser::parsing::Parser<zappy::parser::parsing::GuiConfig> parser(std::make_unique<parser::parsing::GUIStrategy>());

    EXPECT_THROW(
        { [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
        shared::exception::ParsingError);
}

TEST(GuiParsingTest, ParserRejectsUnknownFlags) {
    std::vector<std::string> args{"./zappy_gui", "-p", "4242", "-x", "truc_invalide"};
    auto argv = buildArgv(args);

    parser::parsing::Parser<zappy::parser::parsing::GuiConfig> parser(std::make_unique<parser::parsing::GUIStrategy>());

    EXPECT_THROW(
        { [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
        shared::exception::ParsingError);
}

TEST(GuiParsingTest, ParserRejectsHelpFlag) {
    std::vector<std::string> args{"./zappy_gui", "-h"};
    auto argv = buildArgv(args);

    parser::parsing::Parser<zappy::parser::parsing::GuiConfig> parser(std::make_unique<parser::parsing::GUIStrategy>());

    EXPECT_THROW(
        { [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
        shared::exception::ParsingError);
}
}  // namespace zappy::gui::tests
