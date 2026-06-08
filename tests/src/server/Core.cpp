/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** core tests
*/

#include "Core.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "util/DataStructures.hpp"

namespace zappy::server::tests {

TEST(CoreTest, ConstructionSucceeds) {
    util::Config config;
    config.port = 4243;

    EXPECT_NO_THROW({ const Core core{config}; });
}

TEST(CoreTest, RunCanBeStopped) {
    util::Config config;
    config.port = 4244;
    Core core{config};

    std::thread coreThread([&core]() { core.run(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    core.stop();
    coreThread.join();
    SUCCEED();
}

}  // namespace zappy::server::tests
