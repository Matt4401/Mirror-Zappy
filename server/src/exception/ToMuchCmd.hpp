/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ToMuchCmd
*/

#pragma once

#include "exception/Exception.hpp"

namespace zappy::server::exception {

class ToMuchCmd : public shared::exception::Exception {
  public:
    using Exception::Exception;
};

}  // namespace zappy::server::exception
