/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Socket exception
*/

#pragma once

#include "include/exception/Exception.hpp"

namespace network::exception {

class SocketError : public zappy::shared::exception::Exception {
  public:
    using Exception::Exception;
};

}  // namespace network::exception
