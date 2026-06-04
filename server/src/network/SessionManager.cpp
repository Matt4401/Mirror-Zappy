/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Session manager
*/

#include "SessionManager.hpp"

#include <sys/poll.h>

#include <cerrno>
#include <cstddef>
#include <cstdint>

#include "exception/PollError.hpp"

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
        throw exception::PollError{"fatal error during poll"};
    }
    if (readyFds == 0) {
        return;
    }
    for (std::size_t i = _pollFds.size(); i > 0; i--) {
        const struct pollfd pfd = _pollFds.at(i - 1);
        if (pfd.revents == 0) {
            continue;
        }

        if (pfd.fd == _serverSocket.fd()) {
            handleServerEvent(pfd.revents);
        } else {
            handleClientEvent(pfd);
        }
    }
}

}  // namespace zappy::server::network
