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
#include "network/IClientSocket.hpp"

namespace network::socket {

constexpr auto kMaxBufferSize = 42000;

constexpr auto delimiter = "\n";

class ClientSocket : public BaseSocket, public zappy::shared::network::IClientSocket {
  public:
    ClientSocket() = delete;
    ClientSocket(std::string_view host, std::uint16_t port);
    explicit ClientSocket(int fd);

    [[nodiscard]] std::size_t send(std::string_view message) const override;
    [[nodiscard]] std::string receive() const override;

    [[nodiscard]] std::optional<std::string> tryPopMessage() override;

  private:
    void connectToServer(std::string_view host, std::uint16_t port);

    std::string _buffer;
};

}  // namespace network::socket
