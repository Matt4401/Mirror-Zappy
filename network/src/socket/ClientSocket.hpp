/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ClientSocket class
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "BaseSocket.hpp"

namespace network::socket {

class ClientSocket : public BaseSocket {
  public:
    ClientSocket() = delete;
    ClientSocket(std::string_view host, std::uint16_t port);
    explicit ClientSocket(int fd);

    [[nodiscard]] std::size_t send(std::string_view message) const;
    [[nodiscard]] std::string receive() const;

    [[nodiscard]] std::optional<std::string> tryPopMessage();

  private:
    void connectToServer(std::string_view host, std::uint16_t port);

    std::string _buffer;
};

}  // namespace network::socket
