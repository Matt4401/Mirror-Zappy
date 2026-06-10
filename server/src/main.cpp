/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <exception>
#include <iostream>

#include "Core.hpp"
#include "parsing/Parser.hpp"
#include "parsing/strategy/ServerStrategy.hpp"
#include "util/DataStructures.hpp"

// NOLINTNEXTLINE
int main(int ac, char** av) {
    try {
        auto serverStrategy = std::make_unique<zappy::shared::parsing::ServerStrategy>();
        zappy::shared::parsing::Parser<zappy::server::util::ServerConfig> parser(std::move(serverStrategy));
        const zappy::server::util::ServerConfig config = parser.parse(ac, av);
        zappy::server::Core core(config);

        core.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
