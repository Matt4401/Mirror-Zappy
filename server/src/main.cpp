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
    } catch (const zappy::shared::exception::Exception& e) {
        if (std::string(e.what()) == "Help displayed") {
        return zappy::shared::parsing::kExitSuccess;
        }
        std::cerr << "Error: " << e.what() << std::endl;
        return zappy::shared::parsing::kExitFailure;
    } catch (const std::exception& e) {
        std::cerr << "Unknow error: " << e.what() << std::endl;
        return zappy::shared::parsing::kExitFailure;
    }
    return zappy::shared::parsing::kExitSuccess;
}
