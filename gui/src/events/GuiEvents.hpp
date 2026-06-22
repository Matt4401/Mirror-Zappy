/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GuiEvents
*/

#pragma once

#include <string>

#include "Color.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::events {
struct PlayerClicked {
    int playerId{0};
    std::string teamName;
    std::string playerName;
    raylib::rmath::Vector3 position;
    raylib::Color teamColor{255, 255, 255, 255};
    std::string textureId;
};

struct SendCommand {
    std::string payload;
};

struct PlayerNameChanged {
    int playerId;
    std::string newName;

struct TileClicked {
    int x{0};
    int y{0};
};
}  // namespace zappy::gui::events
