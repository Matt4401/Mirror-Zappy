/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** logger
*/

#pragma once

#include <string_view>
namespace zappy::server {

class Logger {
  public:
    static void logInfo(std::string_view message);
    static void logError(std::string_view message);
};

}  // namespace zappy::server
