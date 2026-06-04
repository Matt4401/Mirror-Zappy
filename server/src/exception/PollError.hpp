/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Poll exception
*/

#pragma once

#include "exception/Exception.hpp"

namespace zappy::server::exception {

class PollError : public shared::exception::Exception {
  public:
    using Exception::Exception;
};

}  // namespace zappy::server::exception
