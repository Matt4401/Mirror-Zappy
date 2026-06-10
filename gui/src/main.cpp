/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <iostream>
#include <memory>

#include "parsing/Parser.hpp"
#include "parsing/strategy/GUIStrategy.hpp"
#include "rcore/Window.hpp"
#include "util/DataStructures.hpp"

// NOLINTNEXTLINE
int main(int ac, char** av) {
    try {
        auto guiStrategy = std::make_unique<zappy::shared::parsing::GUIStrategy>();
        zappy::shared::parsing::Parser<zappy::gui::util::GUIConfig> parser(std::move(guiStrategy));
        const zappy::gui::util::GUIConfig config = parser.parse(ac, av);
        zappy::gui::raylib::rcore::Window window(800, 600, "Zappy GUI");

        window.setTargetFPS(60);
        while (!window.shouldClose()) {
        }
        std::cout << "Hey, I am a zappy gui!" << std::endl;
    } catch (const zappy::shared::exception::ParsingError& e) {
        if (std::string(e.what()) == "Help displayed") {
            return 0;
        }
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    } catch (const std::exception& e) {
        std::cerr << "Unknow error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
