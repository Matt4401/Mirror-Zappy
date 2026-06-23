/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <cstddef>
#include <exception>
#include <iostream>
#include <span>

#include "Core.hpp"
#include "Parser.hpp"

// NOLINTNEXTLINE
int main(int ac, char** av) {
    const auto args = std::span{av, static_cast<std::size_t>(ac)};

    try {
        return zappy::server::Core(args).run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return zappy::parser::kExitFailure;
    }
}
