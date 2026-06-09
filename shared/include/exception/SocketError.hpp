/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Socket exception
*/

#pragma once

#include "Exception.hpp"

namespace zappy::shared::exception {

class SocketError : public Exception {
  public:
    using Exception::Exception;
};

}  // namespace zappy::shared::exception
