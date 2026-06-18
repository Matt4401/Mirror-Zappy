/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ParsingError
*/

#pragma once

#include <format>
#include <string>
#include <string_view>

#include "Exception.hpp"

namespace zappy::shared::exception {

class ParsingError : public Exception {
  public:
    static constexpr std::string_view PREFIX = "[Parsing error]: ";

    template <typename... Args>
    explicit ParsingError(const std::string_view fmt, Args... args, const source &location = source::current())
        : Exception(std::string(PREFIX) + std::vformat(fmt, std::make_format_args(args...)), location) {}

    explicit ParsingError(const std::string_view msg, const source &location = source::current())
        : Exception(std::string(PREFIX) + std::string(msg), location) {}
};

#define PARSING_ERROR(fmt, ...) \
    ParsingError(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), std::source_location::current())
}  // namespace zappy::shared::exception
