/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Server socket (binds and accepts connections)
*/

#pragma once

#include <cstdint>
#include <optional>

#include "BaseSocket.hpp"
#include "ClientSocket.hpp"

namespace network::socket {

class ServerSocket : public BaseSocket {
  public:
    ServerSocket() = delete;
    explicit ServerSocket(std::uint16_t port);

    [[nodiscard]] std::optional<ClientSocket> acceptClient() const;

  private:
    void bindAndListen(std::uint16_t port);
};

}  // namespace network::socket
