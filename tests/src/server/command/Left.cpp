/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Left
*/

#include "command/Left.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <memory>
#include <string>

#include "command/Forward.hpp"
#include "command/ICommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "strategy/ServerStrategy.hpp"

namespace zappy::server::command {
TEST(LeftTest, CheckRequiredTicks) {
    const std::unique_ptr<ICommand> left = std::make_unique<Left>();
    const auto ticks = left->requiredTicks();
    ASSERT_EQ(ticks, 7);
}

TEST(LeftTest, CheckTurnMovement) {
    const std::unique_ptr<ICommand> left = std::make_unique<Left>();
    const std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    game::Player player{0, 5, 5, game::cardinalPoint::NORTH};
    const auto config = parser::ServerConfig{
        .port = 80, .width = 16, .height = 16, .teamNames = {"test"}, .clientLimit = 1, .freq = 100};
    game::World world{config};

    left->execute(world, player);
    const auto newOrient = player.orientation();
    ASSERT_EQ(newOrient, game::cardinalPoint::WEST);
    forward->execute(world, player);
    auto [fst, snd] = player.position();
    ASSERT_EQ(fst, 4);
    ASSERT_EQ(snd, 5);
}

TEST(LeftTest, CheckTurnMovementBordure) {
    const std::unique_ptr<ICommand> left = std::make_unique<Left>();
    const std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    const auto config = parser::ServerConfig{
        .port = 80, .width = 16, .height = 16, .teamNames = {"test"}, .clientLimit = 1, .freq = 100};
    game::World world{config};
    auto [maxX, maxY] = world.sizeMap();
    game::Player player{0, maxX, maxY, game::cardinalPoint::NORTH};

    left->execute(world, player);
    ASSERT_EQ(player.orientation(), game::cardinalPoint::WEST);
    forward->execute(world, player);
    auto [fst, snd] = player.position();
    ASSERT_EQ(fst, (maxX + static_cast<std::size_t>(-1) + (maxX + 1)) % (maxX + 1));
    ASSERT_EQ(snd, maxY);
}
}  // namespace zappy::server::command
