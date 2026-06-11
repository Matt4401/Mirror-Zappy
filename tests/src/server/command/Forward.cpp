/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** forward command tests
*/

#include "command/Forward.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "command/ICommand.hpp"
#include "game/Player.hpp"
#include "util/DataStructures.hpp"

namespace zappy::server::command {

TEST(ForwardTest, CheckRequiredTicks) {
    const std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    const auto ticks = forward->requiredTicks();
    ASSERT_EQ(ticks, 7);
}

TEST(ForwardTest, CheckMovement) {
    std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    game::Player player{0, 5, 5};
    const util::Config config{};
    game::World world{config};

    forward->execute(world, player);
    auto [fst, snd] = player.position();
    ASSERT_EQ(fst, 5);
    ASSERT_EQ(snd, 6);
}

TEST(ForwardTest, CheckMovementBordure) {
    const std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    auto [maxX, maxY] = game::World::limitMap();
    game::Player player{0, maxX, maxY};
    const util::Config config{};
    game::World world{config};

    forward->execute(world, player);
    auto [fst, snd] = player.position();
    ASSERT_EQ(fst, 16);
    ASSERT_EQ(snd, 0);
}
}  // namespace zappy::server::command
