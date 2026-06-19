/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** exceptions
*/

#include "exception/Exception.hpp"

#include <string>
#include <utility>

namespace zappy::shared::exception {

Exception::Exception(std::string message) : _message{std::move(message)} {}

const char* Exception::what() const noexcept { return _message.c_str(); }

}  // namespace zappy::shared::exception
