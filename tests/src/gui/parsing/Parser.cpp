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
#include "../../gui/src/util/DataStructures.hpp"

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

        shared::parsing::Parser<util::GUIConfig> parser(
               std::make_unique<shared::parsing::GUIStrategy>());

        const util::GUIConfig config = parser.parse(static_cast<int>(argv.size()), argv.data());

        EXPECT_EQ(config.port, 4242);
        EXPECT_EQ(config.machine, "192.168.1.50");
}

TEST(GuiParsingTest, ParserAppliesDefaultMachineWhenFlagIsMissing) {
        std::vector<std::string> args{"./zappy_gui", "-p", "8080"};
        auto argv = buildArgv(args);

        shared::parsing::Parser<util::GUIConfig> parser(
               std::make_unique<shared::parsing::GUIStrategy>());

        const util::GUIConfig config = parser.parse(static_cast<int>(argv.size()), argv.data());

        EXPECT_EQ(config.port, 8080);
        EXPECT_EQ(config.machine, "localhost");
}

TEST(GuiParsingTest, ParserRejectsMissingPort) {
        std::vector<std::string> args{"./zappy_gui", "-h", "127.0.0.1"};
        auto argv = buildArgv(args);

        shared::parsing::Parser<util::GUIConfig> parser(
               std::make_unique<shared::parsing::GUIStrategy>());

        EXPECT_THROW({ [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
                              shared::exception::ParsingError);
}

TEST(GuiParsingTest, ParserRejectsInvalidPorts) {
        std::vector<std::string> args{"./zappy_gui", "-p", "-5", "-h", "localhost"};
        auto argv = buildArgv(args);

        shared::parsing::Parser<util::GUIConfig> parser(
               std::make_unique<shared::parsing::GUIStrategy>());

        EXPECT_THROW({ [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
                              shared::exception::ParsingError);
}

TEST(GuiParsingTest, ParserRejectsNonNumericPort) {
        std::vector<std::string> args{"./zappy_gui", "-p", "not_a_number"};
        auto argv = buildArgv(args);

        shared::parsing::Parser<util::GUIConfig> parser(
               std::make_unique<shared::parsing::GUIStrategy>());

        EXPECT_THROW({ [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
                              shared::exception::ParsingError);
}

TEST(GuiParsingTest, ParserRejectsUnknownFlags) {
        std::vector<std::string> args{"./zappy_gui", "-p", "4242", "-x", "truc_invalide"};
        auto argv = buildArgv(args);

        shared::parsing::Parser<util::GUIConfig> parser(
               std::make_unique<shared::parsing::GUIStrategy>());

        EXPECT_THROW({ [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
                              shared::exception::ParsingError);
}

TEST(GuiParsingTest, ParserPrintsHelpWhenHelpFlagIsPresent) {
        std::vector<std::string> args{"./zappy_gui", "--help"};
        auto argv = buildArgv(args);

        shared::parsing::Parser<util::GUIConfig> parser(
               std::make_unique<shared::parsing::GUIStrategy>());

        testing::internal::CaptureStdout();

        const std::string output = testing::internal::GetCapturedStdout();

        EXPECT_NE(output.find("USAGE: ./zappy_gui -p port -h machine"), std::string::npos);
}
}  // namespace zappy::gui::tests