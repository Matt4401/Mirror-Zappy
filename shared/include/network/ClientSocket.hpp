/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ClientSocket class
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

#include "BaseSocket.hpp"

namespace zappy::shared::network {

class ClientSocket : public BaseSocket {
  public:
    ClientSocket() = delete;
    ClientSocket(std::string_view host, std::uint16_t port);
    explicit ClientSocket(int fd);

    [[nodiscard]] std::size_t send(std::string_view message) const;
    [[nodiscard]] std::string receive() const;

    void setNonBlocking() const;

  private:
    void connectToServer(std::string_view host, std::uint16_t port);
};

}  // namespace zappy::shared::network
