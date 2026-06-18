/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** socket class
*/

#include "ClientSocket.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <array>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "BaseSocket.hpp"
#include "exception/ConnectError.hpp"
#include "exception/SocketError.hpp"

namespace network::socket {

ClientSocket::ClientSocket(const int fd) : BaseSocket{fd} { setNonBlocking(); }

ClientSocket::ClientSocket(std::string_view host, const std::uint16_t port) {
    connectToServer(host, port);
    setNonBlocking();
}

void ClientSocket::connectToServer(std::string_view host, const std::uint16_t port) {
    const std::string hostStr{host};
    sockaddr_in servAddr{};
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = ::htons(port);

    if (::inet_pton(AF_INET, hostStr.c_str(), &servAddr.sin_addr) <= 0) {
        throw exception::SocketError("invalid address or hostname not supported");
    }
    const int newFd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (newFd < 0) {
        throw exception::SocketError("failed to create socket");
    }
    setFd(newFd);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (::connect(fd(), reinterpret_cast<const sockaddr*>(&servAddr), sizeof(servAddr)) < 0) {
        if (errno == ECONNREFUSED) {
            throw exception::ConnectError("connection refused");
        }
        if (errno == ETIMEDOUT) {
            throw exception::ConnectError("connection timeout");
        }
        throw exception::SocketError("cannot connect: unknown error");
    }
}

std::size_t ClientSocket::send(std::string_view message) const {
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

std::optional<std::string> ClientSocket::tryPopMessage() {
    std::string newData;
    try {
        newData = receive();
    } catch (const exception::SocketError& e) {
        if (std::string_view{e.what()} == "client disconnected") {
            throw;
        }
        return std::nullopt;
    }

    if (newData.size() + _buffer.size() > 8192) {
        throw exception::SocketError{"read buffer overflow, disconnecting client"};
    }
    _buffer += newData;
    const std::size_t newlinePos = _buffer.find(delimiter);

    if (newlinePos == std::string::npos) {
        return std::nullopt;
    }
    std::string message = _buffer.substr(0, newlinePos);
    if (!message.empty() && message.back() == '\r') {
        message.pop_back();
    }
    _buffer.erase(0, newlinePos + 1);
    return message;
}

}  // namespace network::socket
