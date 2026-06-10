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
void Core::run() {
    _client = std::make_unique<network::Client>(_config);
    _render = std::make_unique<graphics::Render>();
    _render->start();
}
}  // namespace zappy::gui
