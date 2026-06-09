/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Connect exception
*/

#pragma once

#include "Exception.hpp"

namespace zappy::shared::exception {

class ConnectError : public Exception {
  public:
    using Exception::Exception;
};

}  // namespace zappy::shared::exception
