/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <cstddef>
#include <span>

#include "Core.hpp"

// NOLINTNEXTLINE
int main(int ac, char** av) {
    const auto args = std::span{av, static_cast<std::size_t>(ac)};

    return zappy::server::Core(args).run();
}
