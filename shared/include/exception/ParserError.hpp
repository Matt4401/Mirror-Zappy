/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** ParserError
*/

#pragma once
#include "Exception.hpp"

namespace zappy::shared::exception {
class ParserError : public Exception {
  public:
    using Exception::Exception;
};
}  // namespace zappy::shared::exception
