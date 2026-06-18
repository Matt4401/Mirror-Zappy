/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ToMuchCmd
*/

#pragma once

#include <format>
#include <string>
#include <string_view>

#include "exception/Exception.hpp"

namespace zappy::server::exception {

class TooMuchCmd : public shared::exception::Exception {
  public:
    static constexpr std::string_view PREFIX = "[TooMuchCmd error]: ";

    template <typename... Args>
    explicit TooMuchCmd(const std::string_view fmt, Args... args,
                        const shared::exception::source &location = shared::exception::source::current())
        : Exception(std::string(PREFIX) + std::vformat(fmt, std::make_format_args(args...)), location) {}

    explicit TooMuchCmd(const std::string_view msg,
                        const shared::exception::source &location = shared::exception::source::current())
        : Exception(std::string(PREFIX) + std::string(msg), location) {}
};

#define TOMUCHCMD_ERROR(fmt, ...) \
    TooMuchCmd(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), std::source_location::current())
}  // namespace zappy::server::exception
