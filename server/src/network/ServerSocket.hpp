/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Server socket (binds and accepts connections)
*/

#pragma once

#include <cstdint>

#include "network/BaseSocket.hpp"
#include "network/ClientSocket.hpp"

namespace zappy::server::network {

class ServerSocket : public shared::network::BaseSocket {
  public:
    ServerSocket() = delete;
    explicit ServerSocket(std::uint16_t port);

    [[nodiscard]] shared::network::ClientSocket acceptClient() const;

  private:
    void bindAndListen(std::uint16_t port);
};

}  // namespace zappy::server::network
