/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Core
*/

#include "Core.hpp"

#include <exception>
#include <iostream>

namespace zappy::gui {
int Core::run() {
    try {
        if (!_cliParser.parse()) {
            return 0;
        }
        _render.start();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
}  // namespace zappy::gui
