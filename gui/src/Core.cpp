/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Core
*/

#include "Core.hpp"

#include <exception>
#include <iostream>
#include <memory>

#include "graphics/Render.hpp"
#include "network/Client.hpp"

namespace zappy::gui {
int Core::run() {
    try {
        if (!_cliParser.parse()) {
            return 0;
        }
        _client = std::make_unique<network::Client>(_cliParser.config());
        _render = std::make_unique<graphics::Render>();
        _render->start();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
}  // namespace zappy::gui
