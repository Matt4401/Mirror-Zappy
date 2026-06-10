/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Parser
*/

#include "parsing/Parser.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include "exception/ParsingError.hpp"
#include "parsing/strategy/ServerStrategy.hpp"
#include "util/DataStructures.hpp"

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
	std::vector<std::string> args{"./zappy_server", "-p", "4242", "-x", "10", "-y", "20", "-n", "TeamA",
								  "TeamB", "-c", "6", "-f", "100"};
	auto argv = buildArgv(args);

	shared::parsing::Parser<util::ServerConfig> parser(
		std::make_unique<shared::parsing::ServerStrategy>());

	const util::ServerConfig config = parser.parse(static_cast<int>(argv.size()), argv.data());

	EXPECT_EQ(config.port, 4242);
	EXPECT_EQ(config.width, 10u);
	EXPECT_EQ(config.height, 20u);
	ASSERT_EQ(config.teamNames.size(), 2u);
	EXPECT_EQ(config.teamNames[0], "TeamA");
	EXPECT_EQ(config.teamNames[1], "TeamB");
	EXPECT_EQ(config.clientLimit, 6u);
	EXPECT_EQ(config.freq, 100u);
}

TEST(ServerParsingTest, ParserRejectsMissingTeamName) {
	std::vector<std::string> args{"./zappy_server", "-p", "4242", "-x", "10", "-y", "20", "-c", "6",
								  "-f", "100"};
	auto argv = buildArgv(args);

	shared::parsing::Parser<util::ServerConfig> parser(
		std::make_unique<shared::parsing::ServerStrategy>());

	EXPECT_THROW({ [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
				 shared::exception::ParsingError);
}

TEST(ServerParsingTest, ParserRejectsReservedGraphicTeamName) {
	std::vector<std::string> args{"./zappy_server", "-p", "4242", "-x", "10", "-y", "20", "-n",
								  "GRAPHIC", "-c", "6", "-f", "100"};
	auto argv = buildArgv(args);

	shared::parsing::Parser<util::ServerConfig> parser(
		std::make_unique<shared::parsing::ServerStrategy>());

	EXPECT_THROW({ [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
				 shared::exception::ParsingError);
}

TEST(ServerParsingTest, ParserPrintsHelpWithoutThrowing) {
	std::vector<std::string> args{"./zappy_server", "--help"};
	auto argv = buildArgv(args);

	shared::parsing::Parser<util::ServerConfig> parser(
		std::make_unique<shared::parsing::ServerStrategy>());

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW({ [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); });
	const std::string output = testing::internal::GetCapturedStdout();

	EXPECT_EQ(output, "USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq\n");
}

TEST(ServerParsingTest, ParserAppliesDefaultFrequencyWhenFlagIsMissing) {
    std::vector<std::string> args{"./zappy_server", "-p", "4242", "-x", "10", "-y", "20", "-n", "TeamA", "-c", "6"};
    auto argv = buildArgv(args);

    shared::parsing::Parser<util::ServerConfig> parser(
           std::make_unique<shared::parsing::ServerStrategy>());

    const util::ServerConfig config = parser.parse(static_cast<int>(argv.size()), argv.data());

    EXPECT_EQ(config.freq, 100u);
}

TEST(ServerParsingTest, ParserRejectsInvalidOrZeroDimensions) {
    std::vector<std::string> args{"./zappy_server", "-p", "4242", "-x", "0", "-y", "20", "-n", "TeamA", "-c", "6"};
    auto argv = buildArgv(args);

    shared::parsing::Parser<util::ServerConfig> parser(
           std::make_unique<shared::parsing::ServerStrategy>());

    EXPECT_THROW({ [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
                          shared::exception::ParsingError);
}

TEST(ServerParsingTest, ParserRejectsNegativePort) {
    std::vector<std::string> args{"./zappy_server", "-p", "-80", "-x", "10", "-y", "20", "-n", "TeamA", "-c", "6"};
    auto argv = buildArgv(args);

    shared::parsing::Parser<util::ServerConfig> parser(
           std::make_unique<shared::parsing::ServerStrategy>());

    EXPECT_THROW({ [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
                          shared::exception::ParsingError);
}

TEST(ServerParsingTest, ParserCorrectlySeparatesTeamsFromFollowingFlags) {
    std::vector<std::string> args{"./zappy_server", "-p", "4242", "-x", "10", "-y", "20", "-n", "TeamA", "TeamB", "-c", "6"};
    auto argv = buildArgv(args);

    shared::parsing::Parser<util::ServerConfig> parser(
           std::make_unique<shared::parsing::ServerStrategy>());

    const util::ServerConfig config = parser.parse(static_cast<int>(argv.size()), argv.data());

    ASSERT_EQ(config.teamNames.size(), 2u);
    EXPECT_EQ(config.teamNames[0], "TeamA");
    EXPECT_EQ(config.teamNames[1], "TeamB");
    EXPECT_EQ(config.clientLimit, 6u);
}

TEST(ServerParsingTest, ParserRejectsNonNumericArguments) {
    std::vector<std::string> args{"./zappy_server", "-p", "poulet", "-x", "10", "-y", "20", "-n", "TeamA", "-c", "6"};
    auto argv = buildArgv(args);

    shared::parsing::Parser<util::ServerConfig> parser(
           std::make_unique<shared::parsing::ServerStrategy>());

    EXPECT_THROW({ [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
                          shared::exception::ParsingError);
}

TEST(ServerParsingTest, ParserRejectsUnknownFlags) {
    std::vector<std::string> args{"./zappy_server", "-k", "random", "-p", "4242", "-x", "10", "-y", "20", "-n", "TeamA", "-c", "6"};
    auto argv = buildArgv(args);

    shared::parsing::Parser<util::ServerConfig> parser(
           std::make_unique<shared::parsing::ServerStrategy>());

    EXPECT_THROW({ [[maybe_unused]] const auto config = parser.parse(static_cast<int>(argv.size()), argv.data()); },
                          shared::exception::ParsingError);
}
}  // namespace zappy::server::tests

