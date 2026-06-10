/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** client socket interface
*/

#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
namespace zappy::shared::network {

class IClientSocket {
  public:
    IClientSocket() = default;
    IClientSocket(const IClientSocket&) = delete;
    IClientSocket& operator=(const IClientSocket&) = delete;
    IClientSocket(IClientSocket&&) noexcept = default;
    IClientSocket& operator=(IClientSocket&&) noexcept = default;
    virtual ~IClientSocket() = default;

    [[nodiscard]] virtual std::size_t send(std::string_view message) const = 0;
    [[nodiscard]] virtual std::string receive() const = 0;

    [[nodiscard]] virtual std::optional<std::string> tryPopMessage() = 0;
};

}  // namespace zappy::shared::network
