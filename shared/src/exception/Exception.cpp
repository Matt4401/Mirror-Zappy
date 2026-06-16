/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** exceptions
*/

#include "exception/Exception.hpp"

#include <sstream>
#include <string>

namespace zappy::shared::exception {

// Exception::Exception(std::string message) : _message{std::move(message)} {}
//
// const char* Exception::what() const noexcept { return _message.c_str(); }

/**
 * @brief Builds an exception with a message and captured source location.
 * @param message Human-readable error description.
 * @param location Call site metadata, defaults to
 * std::source_location::current().
 */
Exception::Exception(const std::string_view message, const source &location) : _message(message), _location(location) {
    _fullMessage = buildFullMessage();
}

/**
 * @brief Returns the fully formatted exception message.
 * @return Null-terminated message buffer owned by this exception instance.
 */
const char *Exception::what() const noexcept { return _fullMessage.c_str(); }

/**
 * @brief Returns the source location associated with this exception.
 * @return Reference to the stored std::source_location.
 */
const source &Exception::location() const noexcept { return _location; }

/**
 * @brief Composes a full message including file and line information.
 * @return Formatted string: "<message> in <file> at line <line>".
 */
std::string Exception::buildFullMessage() const {
    std::ostringstream oss;
    oss << _message << " in " << _location.file_name() << " at line " << _location.line();
    return oss.str();
}

}  // namespace zappy::shared::exception
