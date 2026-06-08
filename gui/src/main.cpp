/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <iostream>

#include "rcore/Camera.hpp"
#include "rcore/Window.hpp"

// NOLINTNEXTLINE
int main(int ac, char** av) {
    zappy::gui::raylib::rcore::Window window(800, 600, "Zappy GUI");

    window.setTargetFPS(60);
    while (!window.shouldClose()) {
    }

    std::cout << "Hey, I am a zappy gui!" << std::endl;
    return 0;
}
