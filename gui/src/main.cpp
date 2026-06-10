/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <iostream>
#include <memory>

#include "Core.hpp"
#include "parsing/Parser.hpp"
#include "parsing/strategy/GUIStrategy.hpp"

// NOLINTNEXTLINE
int main(int ac, char** av) {
    try {
        auto guiStrategy = std::make_unique<zappy::parser::parsing::GUIStrategy>();
        zappy::parser::parsing::Parser<zappy::parser::parsing::GuiConfig> parser(std::move(guiStrategy));
        const zappy::parser::parsing::GuiConfig config = parser.parse(ac, av);
        zappy::gui::Core core(config);

        core.run();
    } catch (const zappy::shared::exception::Exception& e) {
        if (std::string(e.what()) == zappy::parser::parsing::kUsageThrowMessage) {
            return zappy::parser::parsing::kExitSuccess;
        }
        std::cerr << "Error: " << e.what() << std::endl;
        return zappy::parser::parsing::kExitFailure;
    } catch (const std::exception& e) {
        std::cerr << "Unknow error: " << e.what() << std::endl;
        return zappy::parser::parsing::kExitFailure;
    }
    return zappy::parser::parsing::kExitSuccess;
}
