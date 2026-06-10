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
#include "util/DataStructures.hpp"

namespace zappy::server::command {
TEST(RightTest, CheckRequiredTicks) {
    const std::unique_ptr<ICommand> right = std::make_unique<Right>();
    const auto ticks = right->requiredTicks();
    ASSERT_EQ(ticks, 7);
}

TEST(RightTest, CheckTurnMovement) {
    const std::unique_ptr<ICommand> right = std::make_unique<Right>();
    const std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    game::Player player{0, 5, 5};
    const util::Config config{};
    game::World world{config};

    right->execute(world, player);
    const auto newOrient = player.getOrientation();
    ASSERT_EQ(newOrient, game::orientation::EAST);
    forward->execute(world, player);
    auto [fst, snd] = player.getPosition();
    ASSERT_EQ(fst, 6);
    ASSERT_EQ(snd, 5);
}

TEST(RightTest, CheckTurnMovementBordure) {
    const std::unique_ptr<ICommand> right = std::make_unique<Right>();
    const std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    game::Player player{0, 16, 16};
    const util::Config config{};
    game::World world{config};

    right->execute(world, player);
    ASSERT_EQ(player.getOrientation(), game::orientation::EAST);
    forward->execute(world, player);
    auto [fst, snd] = player.getPosition();
    ASSERT_EQ(fst, 0);
    ASSERT_EQ(snd, 16);
}
}  // namespace zappy::server::command
