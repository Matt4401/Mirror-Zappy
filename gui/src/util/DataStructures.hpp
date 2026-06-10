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

struct Config {
    std::uint16_t port = 0;
    std::string machine = "127.0.0.1";
};

}  // namespace zappy::gui::util
