/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** core tests
*/

#include "Core.hpp"

#include <gtest/gtest.h>

#include <span>
#include <string>
#include <vector>

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

TEST(CoreTest, ConstructionSucceeds) {
    std::vector<std::string> args{"./zappy_server", "-p", "4243", "-x", "10", "-y", "10", "-n", "TeamA", "-c", "1"};
    auto argv = buildArgv(args);
    const std::span<char*> argvSpan{argv.data(), argv.size()};

    EXPECT_NO_THROW({ const Core core{argvSpan}; });
}

}  // namespace zappy::server::tests
