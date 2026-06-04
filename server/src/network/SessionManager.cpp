/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Session manager
*/

#include "SessionManager.hpp"

#include <sys/poll.h>

#include <cerrno>
#include <cstdint>
#include <stdexcept>

namespace zappy::server::network {

SessionManager::SessionManager(std::uint16_t port) {
    _serverSocket.bindAndListen(port);
    _pollFds.push_back({.fd = _serverSocket.fd(), .events = POLLIN});
}

void SessionManager::pollNetwork() {
    const int readyFds = ::poll(_pollFds.data(), _pollFds.size(), 0);

    if (readyFds < 0) {
        if (errno == EINTR) {
            return;
        }
        throw std::runtime_error{"fatal error during poll"};
    }
    if (readyFds == 0) {
        return;
    }
}

}  // namespace zappy::server::network
