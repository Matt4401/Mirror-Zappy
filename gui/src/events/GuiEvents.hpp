/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GuiEvents
*/

#pragma once

#include <cstdint>
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
};

struct PlayerFirstPersonRequested {
    int playerId{0};
};

struct TileClicked {
    int x{0};
    int y{0};
};

struct RequestCyclePlayer {
    int direction{0};
    int currentPlayerId{-1};
};

struct PlayerUnselected {};

enum class TimeMode : std::uint8_t { MORNING, AFTERNOON, EVENING, NIGHT, CYCLE };

struct TimeOfDayChanged {
    TimeMode mode;
};

}  // namespace zappy::gui::events
