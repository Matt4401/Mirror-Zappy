/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Poll exception
*/

#pragma once

#include "include/exception/Exception.hpp"

namespace network::exception {

class PollError : public zappy::shared::exception::Exception {
  public:
    using Exception::Exception;
};

}  // namespace network::exception
