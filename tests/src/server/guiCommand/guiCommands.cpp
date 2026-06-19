/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** guicommands tests
*/

#include <gtest/gtest.h>

#include <span>
#include <string>
#include <vector>

#include "Core.hpp"
#include "guiCommand/Bct.hpp"
#include "guiCommand/Mct.hpp"
#include "guiCommand/Msz.hpp"
#include "guiCommand/Sgt.hpp"
#include "guiCommand/Tna.hpp"

// NOLINTBEGIN
const auto createDummyArgs = []() {
    static char arg0[] = "./zappy_server";
    static char arg1[] = "-p";
    static char arg2[] = "4242";
    static char arg3[] = "-x";
    static char arg4[] = "10";
    static char arg5[] = "-y";
    static char arg6[] = "20";
    static char arg7[] = "-n";
    static char arg8[] = "teamA";
    static char arg9[] = "teamB";
    static char arg10[] = "-c";
    static char arg11[] = "5";
    static char arg12[] = "-f";
    static char arg13[] = "50";

    return std::vector<char*>{arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13};
};
// NOLINTEND

TEST(MszCommandTest, ExecuteReturnsCorrectDimensions) {
    auto args = createDummyArgs();
    zappy::server::Core core{std::span<char*>(args)};
    core.setup();

    zappy::server::guiCommand::Msz command{};
    const std::string response = command.execute(core);

    EXPECT_EQ(response, "msz 10 20\n");
}

TEST(SgtCommandTest, ExecuteReturnsCorrectFrequency) {
    auto args = createDummyArgs();
    zappy::server::Core core{std::span<char*>(args)};
    core.setup();

    zappy::server::guiCommand::Sgt command{};
    const std::string response = command.execute(core);

    EXPECT_EQ(response, "sgt 50\n");
}

TEST(BctCommandTest, ExecuteReturnsProperlyFormattedTileContent) {
    auto args = createDummyArgs();
    zappy::server::Core core{std::span<char*>(args)};
    core.setup();

    zappy::server::guiCommand::Bct command{5, 5};
    const std::string response = command.execute(core);

    EXPECT_EQ(response, "bct 5 5 0 0 0 0 0 0 0\n");
}

TEST(BctCommandTest, ExecuteFailsSafelyOnOutOfBounds) {
    auto args = createDummyArgs();
    zappy::server::Core core{std::span<char*>(args)};
    core.setup();

    zappy::server::guiCommand::Bct command{999, 999};
    const std::string response = command.execute(core);

    EXPECT_TRUE(response == "sbp\n");
}

TEST(MctCommandTest, ExecuteReturnsAllTileContents) {
    auto args = createDummyArgs();
    zappy::server::Core core{std::span<char*>(args)};
    core.setup();

    zappy::server::guiCommand::Mct command{};
    const std::string response = command.execute(core);

    std::string expectedResponse;
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 20; ++y) {
            expectedResponse += "bct " + std::to_string(x) + " " + std::to_string(y) + " 0 0 0 0 0 0 0\n";
        }
    }
    EXPECT_EQ(response, expectedResponse);
}

TEST(TnaCommandTest, ExecuteReturnsTeamNames) {
    auto args = createDummyArgs();
    zappy::server::Core core{std::span<char*>(args)};
    core.setup();

    zappy::server::guiCommand::Tna command{};
    const std::string response = command.execute(core);

    EXPECT_EQ(response, "tna teamA\ntna teamB\n");
}
