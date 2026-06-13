/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Parser
*/

#include "Parser.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include "exception/ParsingError.hpp"
#include "strategy/ServerStrategy.hpp"

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

namespace zappy::server::tests {

TEST(ServerParsingTest, ParserReturnsExpectedConfigWhenArgumentsAreValid) {
    std::vector<std::string> args{"./zappy_server", "-p",    "4242", "-x", "10", "-y", "20", "-n",
                                  "TeamA",          "TeamB", "-c",   "6",  "-f", "100"};
    auto argv = buildArgv(args);

    parser::Parser<zappy::parser::ServerConfig> parser(std::make_unique<parser::ServerStrategy>());

    const auto [port, width, height, teamNames, clientLimit, freq] =
        parser.parse(static_cast<int>(argv.size()), argv.data());

    EXPECT_EQ(port, 4242);
    EXPECT_EQ(width, 10U);
    EXPECT_EQ(height, 20U);
    ASSERT_EQ(teamNames.size(), 2U);
    EXPECT_EQ(teamNames.at(0), "TeamA");
    EXPECT_EQ(teamNames.at(1), "TeamB");
    EXPECT_EQ(clientLimit, 6U);
    EXPECT_EQ(freq, 100U);
}

TEST(ServerParsingTest, ParserRejectsMissingTeamName) {
    std::vector<std::string> args{"./zappy_server", "-p", "4242", "-x", "10", "-y", "20", "-c", "6", "-f", "100"};
    auto argv = buildArgv(args);

    parser::Parser<zappy::parser::ServerConfig> parser(std::make_unique<parser::ServerStrategy>());

    EXPECT_THROW(
        { [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
        shared::exception::ParsingError);
}

TEST(ServerParsingTest, ParserRejectsReservedGraphicTeamName) {
    std::vector<std::string> args{"./zappy_server", "-p", "4242", "-x", "10", "-y", "20", "-n",
                                  "GRAPHIC",        "-c", "6",    "-f", "100"};
    auto argv = buildArgv(args);

    parser::Parser<zappy::parser::ServerConfig> parser(std::make_unique<parser::ServerStrategy>());

    EXPECT_THROW(
        { [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
        shared::exception::ParsingError);
}

TEST(ServerParsingHelp, ParserDisplaysHelpAndThrows) {
    std::vector<std::string> args{"./zappy_server", "-h"};
    auto argv = buildArgv(args);

    parser::Parser<zappy::parser::ServerConfig> parser(std::make_unique<parser::ServerStrategy>());

    EXPECT_THROW(
        { [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
        shared::exception::ParsingError);
}

TEST(ServerParsingTest, ParserAppliesDefaultFrequencyWhenFlagIsMissing) {
    std::vector<std::string> args{"./zappy_server", "-p", "4242", "-x", "10", "-y", "20", "-n", "TeamA", "-c", "6"};
    auto argv = buildArgv(args);

    parser::Parser<zappy::parser::ServerConfig> parser(std::make_unique<parser::ServerStrategy>());

    const zappy::parser::ServerConfig config = parser.parse(static_cast<int>(argv.size()), argv.data());

    EXPECT_EQ(config.freq, 100U);
}

TEST(ServerParsingTest, ParserRejectsInvalidOrZeroDimensions) {
    std::vector<std::string> args{"./zappy_server", "-p", "4242", "-x", "0", "-y", "20", "-n", "TeamA", "-c", "6"};
    auto argv = buildArgv(args);

    parser::Parser<zappy::parser::ServerConfig> parser(std::make_unique<parser::ServerStrategy>());

    EXPECT_THROW(
        { [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
        shared::exception::ParsingError);
}

TEST(ServerParsingTest, ParserRejectsNegativePort) {
    std::vector<std::string> args{"./zappy_server", "-p", "-80", "-x", "10", "-y", "20", "-n", "TeamA", "-c", "6"};
    auto argv = buildArgv(args);

    parser::Parser<zappy::parser::ServerConfig> parser(std::make_unique<parser::ServerStrategy>());

    EXPECT_THROW(
        { [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
        shared::exception::ParsingError);
}

TEST(ServerParsingTest, ParserCorrectlySeparatesTeamsFromFollowingFlags) {
    std::vector<std::string> args{"./zappy_server", "-p",    "4242", "-x", "10", "-y", "20", "-n",
                                  "TeamA",          "TeamB", "-c",   "6"};
    auto argv = buildArgv(args);

    parser::Parser<zappy::parser::ServerConfig> parser(std::make_unique<parser::ServerStrategy>());

    const zappy::parser::ServerConfig config = parser.parse(static_cast<int>(argv.size()), argv.data());

    ASSERT_EQ(config.teamNames.size(), 2U);
    EXPECT_EQ(config.teamNames.at(0), "TeamA");
    EXPECT_EQ(config.teamNames.at(1), "TeamB");
    EXPECT_EQ(config.clientLimit, 6U);
}

TEST(ServerParsingTest, ParserRejectsNonNumericArguments) {
    std::vector<std::string> args{"./zappy_server", "-p", "poulet", "-x", "10", "-y", "20", "-n", "TeamA", "-c", "6"};
    auto argv = buildArgv(args);

    parser::Parser<zappy::parser::ServerConfig> parser(std::make_unique<parser::ServerStrategy>());

    EXPECT_THROW(
        { [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
        shared::exception::ParsingError);
}

TEST(ServerParsingTest, ParserRejectsUnknownFlags) {
    std::vector<std::string> args{"./zappy_server", "-k", "random", "-p", "4242", "-x", "10", "-y", "20", "-n",
                                  "TeamA",          "-c", "6"};
    auto argv = buildArgv(args);

    parser::Parser<zappy::parser::ServerConfig> parser(std::make_unique<parser::ServerStrategy>());

    EXPECT_THROW(
        { [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
        shared::exception::ParsingError);
}
}  // namespace zappy::server::tests
