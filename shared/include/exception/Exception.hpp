/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** exceptions
*/

#pragma once

#include <exception>
#include <source_location>
#include <string>

namespace zappy::shared::exception {

using source = std::source_location;

class Exception : public std::exception {
public:
    explicit Exception(std::string_view,
                       const source &location = source::current());
    Exception(const Exception &) = default;
    Exception(Exception &&) = default;
    ~Exception() override = default;

    Exception &operator=(const Exception &) = default;
    Exception &operator=(Exception &&) = default;

    [[nodiscard]] const char *what() const noexcept override;
    [[nodiscard]] const source &location() const noexcept;

protected:
    std::string _message;
    std::source_location _location;
    std::string _fullMessage;

    [[nodiscard]] std::string buildFullMessage() const;
};

}  // namespace zappy::shared::exception
