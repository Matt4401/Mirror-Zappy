/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Data structures declaration
*/

#pragma once

#include <cstdint>
#include <string>
#include <vector>
namespace zappy::server::util {

struct ServerConfig {
    std::uint16_t port{};
    std::uint32_t width{};
    std::uint32_t height{};
    std::vector<std::string> teamNames;
    std::uint32_t clientLimit{};
    std::uint32_t freq = 100;
};

}  // namespace zappy::server::util
