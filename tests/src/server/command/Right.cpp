/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Right
*/

#include "command/Right.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "command/Forward.hpp"
#include "command/ICommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server::command {
TEST(RightTest, CheckRequiredTicks) {
    const std::unique_ptr<ICommand> right = std::make_unique<Right>();
    const auto ticks = right->requiredTicks();
    ASSERT_EQ(ticks, 7);
}

TEST(RightTest, CheckTurnMovement) {
    const std::unique_ptr<ICommand> right = std::make_unique<Right>();
    const std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    game::Player player{0, 5, 5, game::cardinalPoint::NORTH};
    const auto config = parser::ServerConfig{
        .port = 80,
        .width = 16,
        .height = 16,
        .teamNames = {"test"},
        .clientLimit = 1,
        .freq = 100,
    };
    game::World world{config};

    right->execute(world, player);
    const auto newOrient = player.orientation();
    ASSERT_EQ(newOrient, game::cardinalPoint::EAST);
    forward->execute(world, player);
    auto [fst, snd] = player.position();
    ASSERT_EQ(fst, 6);
    ASSERT_EQ(snd, 5);
}

TEST(RightTest, CheckTurnMovementBordure) {
    const std::unique_ptr<ICommand> right = std::make_unique<Right>();
    const std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    const auto config = parser::ServerConfig{
        .port = 80,
        .width = 16,
        .height = 16,
        .teamNames = {"test"},
        .clientLimit = 1,
        .freq = 100,
    };
    game::World world{config};
    auto [maxX, maxY] = world.sizeMap();
    game::Player player{0, maxX - 1, maxY - 1, game::cardinalPoint::NORTH};

    right->execute(world, player);
    ASSERT_EQ(player.orientation(), game::cardinalPoint::EAST);
    forward->execute(world, player);
    auto [fst, snd] = player.position();
    ASSERT_EQ(fst, 0);
    ASSERT_EQ(snd, maxY - 1);
}
}  // namespace zappy::server::command
