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

struct Config {
    std::uint16_t port{};
    std::uint32_t width{};
    std::uint32_t height{};
    std::vector<std::string> teamNames;
    std::uint32_t clientLimit{};
    std::uint32_t freq{};
};

}  // namespace zappy::server::util
