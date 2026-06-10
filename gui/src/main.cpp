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
        auto guiStrategy = std::make_unique<zappy::shared::parsing::GUIStrategy>();
        zappy::shared::parsing::Parser<zappy::shared::parsing::GuiConfig> parser(std::move(guiStrategy));
        const zappy::shared::parsing::GuiConfig config = parser.parse(ac, av);
        zappy::gui::Core core(config);

        core.run();
    } catch (const zappy::shared::exception::Exception& e) {
        if (std::string(e.what()) == zappy::shared::parsing::kUsageThrowMessage) {
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
