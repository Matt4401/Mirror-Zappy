/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** socket class
*/

#include "ClientSocket.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>

#include "BaseSocket.hpp"

namespace zappy::shared::network {

ClientSocket::ClientSocket(const int fd) : BaseSocket{fd} {}

std::size_t ClientSocket::send(const std::string_view message) const {
    std::size_t totalSent = 0;
    const std::size_t toSend = message.size();

    while (totalSent < toSend) {
        const std::string_view remaining = message.substr(totalSent);
        const ssize_t bytesSent = ::send(fd(), remaining.data(), remaining.size(), MSG_NOSIGNAL);

        if (bytesSent < 0) {
            if (errno == EINTR) {
                continue;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return totalSent;
            }
            throw std::runtime_error{"failed to send data"};
        }
        if (bytesSent == 0) {
            break;
        }
        totalSent += static_cast<std::size_t>(bytesSent);
    }

    return totalSent;
}

std::string ClientSocket::receive() const {
    std::array<char, 4096> buffer{};
    const ::ssize_t bytesRead = ::recv(fd(), buffer.data(), buffer.size(), 0);

    if (bytesRead < 0) {
        if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
            return "";
        }
        throw std::runtime_error{"failed to receive data"};
    }
    if (bytesRead == 0) {
        throw std::runtime_error{"client disconnected"};
    }

    return std::string{buffer.data(), static_cast<std::size_t>(bytesRead)};
}

}  // namespace zappy::shared::network
