/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** DataStructures
*/

#pragma once

#include <cstdint>
#include <string>

namespace zappy::gui::util {
struct GUIConfig {
    std::uint16_t port{};
    std::string machine = "localhost";
};
}  // namespace zappy::gui::util
