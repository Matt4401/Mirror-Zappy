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

namespace zappy::server::command {

TEST(ForwardTest, CheckRequiredTicks) {
    std::unique_ptr<ICommand> forward = std::make_unique<Forward>();
    auto ticks = forward->requiredTicks();
    ASSERT_EQ(ticks, 7);
}
}  // namespace zappy::server::command
