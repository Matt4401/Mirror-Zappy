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

}  // namespace zappy::server::tests
