/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GuiEvents
*/

#pragma once

#include <string>

#include "rmath/Vector3.hpp"

namespace zappy::gui::events {
struct PlayerClicked {
    int playerId{0};
    std::string teamName;
    std::string playerName;
    raylib::rmath::Vector3 position;
};
}  // namespace zappy::gui::events
