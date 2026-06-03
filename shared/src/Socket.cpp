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

#include <cerrno>
#include <cstddef>
#include <string_view>

#include "exception/SocketError.hpp"

namespace zappy::shared {

void shared::Socket::send(const std::string_view& message) const {
    std::size_t totalSent = 0;
    const std::size_t toSend = message.size();

    while (totalSent < toSend) {
        std::string_view const remaining = message.substr(totalSent);
        ssize_t const bytesSent = ::send(_fd, remaining.data(), remaining.size(), 0);

        if (bytesSent < 0) {
            if (errno == EINTR) {
                continue;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            }
            throw exception::SocketError("Failed to send data.");
        }
        if (bytesSent == 0) {
            break;
        }
        totalSent += static_cast<size_t>(bytesSent);
    }
}

}  // namespace zappy::shared
