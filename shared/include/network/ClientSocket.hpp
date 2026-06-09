/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ClientSocket class
*/

#pragma once

#include <cstddef>
#include <string>
#include <string_view>

#include "BaseSocket.hpp"

namespace zappy::shared::network {

class ClientSocket : public BaseSocket {
  public:
    ClientSocket() = default;
    explicit ClientSocket(int fd);

    [[nodiscard]] std::size_t send(std::string_view message) const;
    [[nodiscard]] std::string receive() const;

    void setNonBlocking() const;
};

}  // namespace zappy::shared::network
