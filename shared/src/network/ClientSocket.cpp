/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** socket class
*/

#include "ClientSocket.hpp"

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <array>
#include <cerrno>
#include <cstddef>
#include <string>
#include <string_view>

#include "BaseSocket.hpp"
#include "exception/SocketError.hpp"

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
            throw exception::SocketError{"failed to send data"};
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
        throw exception::SocketError{"failed to receive data"};
    }
    if (bytesRead == 0) {
        throw exception::SocketError{"client disconnected"};
    }

    return std::string{buffer.data(), static_cast<std::size_t>(bytesRead)};
}

void ClientSocket::setNonBlocking() const {
    const int flags = ::fcntl(fd(), F_GETFL, 0);
    if (flags == -1) {
        throw exception::SocketError{"unable to get socket flags"};
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    if (::fcntl(fd(), F_SETFL, flags | O_NONBLOCK) == -1) {
        throw exception::SocketError{"unable to set non-blocking mode"};
    }
}

}  // namespace zappy::shared::network
