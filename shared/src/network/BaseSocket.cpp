/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Socket base
*/

#include "BaseSocket.hpp"

#include <unistd.h>

#include <utility>

#include "exception/SocketError.hpp"

namespace zappy::shared::network {

BaseSocket::BaseSocket(int fd) : _fd{fd} {}

BaseSocket::~BaseSocket() {
    if (_fd >= 0) {
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
    if (_fd < 0) {
        throw exception::SocketError("Invalid socket file descriptor.");
    }
    return _fd;
}

void BaseSocket::close() {
    if (_fd < 0) {
        throw exception::SocketError("Invalid socket file descriptor.");
    }
    if (_fd >= 0) {
        ::close(_fd);
        _fd = -1;
    }
}

void BaseSocket::setFd(int fd) {
    if (_fd >= 0) {
        ::close(_fd);
    }
    _fd = fd;
}

}  // namespace zappy::shared::network
