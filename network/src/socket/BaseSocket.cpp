/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Socket base
*/

#include "BaseSocket.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <utility>

#include "exception/SocketError.hpp"

namespace network::socket {

BaseSocket::BaseSocket(int fd) : _fd{fd} {}

BaseSocket::~BaseSocket() {
    if (_fd != -1) {
        ::close(_fd);
    }
}

BaseSocket::BaseSocket(BaseSocket&& other) noexcept { *this = std::move(other); }

BaseSocket& BaseSocket::operator=(BaseSocket&& other) noexcept {
    if (this != &other) {
        if (_fd != -1) {
            ::close(_fd);
        }
        _fd = other._fd;
        other._fd = -1;
    }
    return *this;
}

int BaseSocket::fd() const {
    if (_fd == -1) {
        throw exception::SocketError("Invalid socket file descriptor.");
    }
    return _fd;
}

void BaseSocket::close() {
    if (_fd == -1) {
        throw exception::SocketError("Invalid socket file descriptor.");
    }
    ::close(_fd);
    _fd = -1;
}

void BaseSocket::setNonBlocking() const {
    const int flags = ::fcntl(fd(), F_GETFL, 0);
    if (flags == -1) {
        throw exception::SocketError{"unable to get socket flags"};
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    if (::fcntl(fd(), F_SETFL, flags | O_NONBLOCK) == -1) {
        throw exception::SocketError{"unable to set non-blocking mode"};
    }
}

void BaseSocket::setFd(int fd) {
    if (_fd != -1) {
        ::close(_fd);
    }
    _fd = fd;
}

}  // namespace network::socket
