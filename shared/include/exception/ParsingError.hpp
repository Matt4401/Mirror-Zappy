/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ParsingError
*/

#pragma once

#include "Exception.hpp"

namespace zappy::shared::exception {

class ParsingError : public Exception {
  public:
    using Exception::Exception;
};

}  // namespace zappy::shared::exception
