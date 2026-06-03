/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** socket class
*/

#include "Socket.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

#include "exception/SocketError.hpp"

namespace zappy::shared {

Socket::Socket(int fd) : _fd{fd} {}

Socket::~Socket() {
    if (_fd >= 0) {
        ::close(_fd);
    }
}

Socket::Socket(Socket&& other) noexcept { *this = std::move(other); }

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        if (_fd != -1) {
            ::close(_fd);
        }
        _fd = other._fd;
        other._fd = -1;
    }
    return *this;
}

std::size_t shared::Socket::send(const std::string_view& message) const {
    if (_fd < 0) {
        throw exception::SocketError("Invalid socket file descriptor.");
    }
    std::size_t totalSent = 0;
    const std::size_t toSend = message.size();

    while (totalSent < toSend) {
        std::string_view const remaining = message.substr(totalSent);
        ssize_t const bytesSent = ::send(_fd, remaining.data(), remaining.size(), MSG_NOSIGNAL);

        if (bytesSent < 0) {
            if (errno == EINTR) {
                continue;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            throw exception::SocketError("Failed to send data.");
        }
        if (bytesSent == 0) {
            break;
        }
        totalSent += static_cast<size_t>(bytesSent);
    }
    return totalSent;
}

std::string Socket::receive() const {
    if (_fd < 0) {
        throw exception::SocketError("Invalid socket file descriptor.");
    }
    std::array<char, 512> buffer{};
    ssize_t const bytesReceived = ::recv(_fd, buffer.data(), buffer.size(), 0);

    if (bytesReceived < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
            return "";
        }
        throw exception::SocketError{"Failed to receive data."};
    }
    if (bytesReceived == 0) {
        throw exception::SocketError{"Client disconnected."};
    }
    return {buffer.data(), static_cast<size_t>(bytesReceived)};
}

}  // namespace zappy::shared
