/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** socket server
*/

#include "ServerSocket.hpp"

#include <netinet/in.h>
#include <sys/socket.h>

#include <cerrno>
#include <cstdint>
#include <optional>

#include "ClientSocket.hpp"
#include "exception/SocketError.hpp"

namespace network::socket {

ServerSocket::ServerSocket(const std::uint16_t port) { bindAndListen(port); }

void ServerSocket::bindAndListen(const std::uint16_t port) {
    sockaddr_in serverAddress{};
    const int opt{1};
    const int newFd = ::socket(AF_INET, SOCK_STREAM, 0);

    if (newFd == -1) {
        throw exception::SocketError{"failed to create server socket"};
    }
    setFd(newFd);
    setNonBlocking();
    // NOLINTNEXTLINE(misc-include-cleaner)
    if (::setsockopt(fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        throw exception::SocketError{"failed to set socket options"};
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = ::htons(port);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (::bind(fd(), reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        throw exception::SocketError{"failed to bind server socket"};
    }
    if (::listen(fd(), SOMAXCONN) == -1) {
        throw exception::SocketError{"failed to listen on server socket"};
    }
}

std::optional<ClientSocket> ServerSocket::acceptClient() const {
    sockaddr_in clientAddress{};
    socklen_t clientLength{sizeof(clientAddress)};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    const int clientFd = ::accept(fd(), reinterpret_cast<sockaddr*>(&clientAddress), &clientLength);

    if (clientFd == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
            return std::nullopt;
        }
        throw exception::SocketError{"failed to accept client"};
    }

    ClientSocket clientSocket{clientFd};
    clientSocket.setNonBlocking();
    return clientSocket;
}

}  // namespace network::socket
