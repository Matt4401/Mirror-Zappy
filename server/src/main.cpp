/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <exception>
#include <iostream>
#include <stdexcept>

#include "Core.hpp"
#include "util/DataStructures.hpp"

// NOLINTNEXTLINE
int main(int ac, char** av) {
    // TODO: implement argument parsing
    const zappy::server::util::Config config{
        .port = 4242, .width = 10, .height = 10, .teamNames = {"Team1", "Team2"}, .clientLimit = 100, .freq = 1};

    try {
        if (config.freq == 0) {
            throw std::invalid_argument("Frequency must be greater than 0");
        }
        zappy::server::Core core{config};

        core.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
