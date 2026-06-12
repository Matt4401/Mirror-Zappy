/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** forward command tests
*/

#include "command/Forward.hpp"

#include <gtest/gtest.h>

#include <memory>

#include "command/ICommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server::command {

TEST(ForwardTest, CheckRequiredTicks) {
    const std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    const auto ticks = forward->requiredTicks();
    ASSERT_EQ(ticks, 7);
}

TEST(ForwardTest, CheckMovement) {
    const std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    game::Player player{0, 5, 5, game::cardinalPoint::NORTH};
    const auto config = parser::ServerConfig{
        .port = 80, .width = 16, .height = 16, .teamNames = {"test"}, .clientLimit = 1, .freq = 100};
    game::World world{config};

    forward->execute(world, player);
    auto [fst, snd] = player.position();
    ASSERT_EQ(fst, 5);
    ASSERT_EQ(snd, 6);
}

TEST(ForwardTest, CheckMovementBordure) {
    const std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    const auto config = parser::ServerConfig{
        .port = 80, .width = 16, .height = 16, .teamNames = {"test"}, .clientLimit = 1, .freq = 100};
    game::World world{config};

    auto [maxX, maxY] = world.sizeMap();
    game::Player player{0, maxX, maxY, game::cardinalPoint::NORTH};

    forward->execute(world, player);
    auto [fst, snd] = player.position();
    ASSERT_EQ(fst, maxX);
    ASSERT_EQ(snd, 0);
}
}  // namespace zappy::server::command
