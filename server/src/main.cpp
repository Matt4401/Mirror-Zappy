/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "Core.hpp"
#include "exception/Exception.hpp"
#include "parsing/Parser.hpp"
#include "parsing/strategy/ServerStrategy.hpp"

// NOLINTNEXTLINE
int main(int ac, char** av) {
    try {
        auto serverStrategy = std::make_unique<zappy::parser::parsing::ServerStrategy>();
        zappy::parser::parsing::Parser<zappy::parser::parsing::ServerConfig> parser(std::move(serverStrategy));
        const zappy::parser::parsing::ServerConfig config = parser.parse(ac, av);
        zappy::server::Core core(config);

        core.run();
    } catch (const zappy::shared::exception::Exception& e) {
        if (std::string(e.what()) == zappy::parser::parsing::kUsageThrowMessage) {
            return zappy::parser::parsing::kExitSuccess;
        }
        std::cerr << "Error: " << e.what() << std::endl;
        return zappy::parser::parsing::kExitFailure;
    } catch (const std::exception& e) {
        std::cerr << "Unknown error: " << e.what() << std::endl;
        return zappy::parser::parsing::kExitFailure;
    }
    return zappy::parser::parsing::kExitSuccess;
}
