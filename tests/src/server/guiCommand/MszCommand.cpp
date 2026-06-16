/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** msz
*/

#include <gtest/gtest.h>

#include <string>

#include "game/World.hpp"
#include "guiCommand/Msz.hpp"
#include "strategy/ServerStrategy.hpp"

TEST(MszCommandTest, ExecuteReturnsCorrectMapSize) {
    zappy::parser::ServerConfig config{};
    config.width = 25;
    config.height = 16;
    config.clientLimit = 1;
    config.teamNames = {"TeamA"};

    const zappy::server::game::World world{config};
    zappy::server::guiCommand::Msz command{};

    const std::string response = command.execute(world);

    EXPECT_EQ(response, "msz 25 16\n");
}

TEST(MszCommandTest, ExecuteHandlesZeroDimensionsSafely) {
    zappy::parser::ServerConfig config{};
    config.width = 0;
    config.height = 0;
    config.clientLimit = 1;
    config.teamNames = {"TeamA"};

    const zappy::server::game::World world{config};
    zappy::server::guiCommand::Msz command{};

    const std::string response = command.execute(world);

    EXPECT_EQ(response, "msz 0 0\n");
}
