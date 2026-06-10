/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Left
*/

#include "command/Left.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "command/Forward.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "util/DataStructures.hpp"

namespace zappy::server::command {
TEST(LeftTest, CheckRequiredTicks) {
    std::unique_ptr<ICommand> left = std::make_unique<Left>();
    auto ticks = left->requiredTicks();
    ASSERT_EQ(ticks, 7);
}

TEST(LeftTest, CheckTurnMovement) {
    std::unique_ptr<ICommand> left = std::make_unique<Left>();
    std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    game::Player player{0, 5, 5};
    util::Config config{};
    game::World world{config};

    left->execute(world, player);
    auto newOrient = player.getOrientation();
    ASSERT_EQ(newOrient, game::orientation::WEST);
    forward->execute(world, player);
    auto [fst, snd] = player.getPosition();
    ASSERT_EQ(fst, 4);
    ASSERT_EQ(snd, 5);
}

TEST(LeftTest, CheckTurnMovementBordure) {
    std::unique_ptr<ICommand> left = std::make_unique<Left>();
    std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    game::Player player{0, 16, 16};
    const util::Config config{};
    game::World world{config};

    left->execute(world, player);
    ASSERT_EQ(player.getOrientation(), game::orientation::WEST);
    forward->execute(world, player);
    auto [fst, snd] = player.getPosition();
    ASSERT_EQ(fst, 15);
    ASSERT_EQ(snd, 16);
}
}  // namespace zappy::server::command
