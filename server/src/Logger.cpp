/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** logger
*/

#include "Logger.hpp"

#include <iostream>
#include <string_view>

namespace zappy::server {

void Logger::logInfo(std::string_view message) { std::cout << "[SERVER] [INFO] " << message << std::endl; }

void Logger::logError(std::string_view message) { std::cerr << "[SERVER] [ERROR] " << message << std::endl; }

}  // namespace zappy::server
