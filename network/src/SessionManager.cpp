/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Session manager
*/

#include "SessionManager.hpp"

#include <sys/poll.h>

#include <algorithm>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "exception/PollError.hpp"
#include "exception/SocketError.hpp"
#include "socket/ClientSocket.hpp"

namespace network {

SessionManager::SessionManager(std::uint16_t port) : _serverSocket{port} {
    _pollFds.push_back({.fd = _serverSocket.fd(), .events = POLLIN});
}

void SessionManager::pollNetwork(int timeout) {
    const int readyFds = ::poll(_pollFds.data(), _pollFds.size(), timeout);

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

bool SessionManager::tryPopMessage(NetworkEvent& message) {
    if (_eventQueue.empty()) {
        return false;
    }
    message = std::move(_eventQueue.front());
    _eventQueue.pop();
    return true;
}

void SessionManager::sendMessage(int clientId, std::string_view message) {
    if (!_clients.contains(clientId)) {
        return;
    }
    std::string& buffer = _writeBuffers[clientId];

    if (message.size() + buffer.size() > socket::kMaxBufferSize) {
        disconnectClient(clientId);
        throw exception::SocketError{"write buffer overflow, disconnecting client"};
    }
    buffer += message;
    for (auto& pfd : _pollFds) {
        if (pfd.fd == clientId) {
            pfd.events |= POLLOUT;
            break;
        }
    }
}

void SessionManager::disconnectClient(const int clientId) {
    if (!_clients.contains(clientId)) {
        return;
    }
    auto it = std::ranges::find_if(_pollFds.begin(), _pollFds.end(),
                                   [clientId](const struct pollfd& pfd) { return pfd.fd == clientId; });

    if (it != _pollFds.end()) {
        _pollFds.erase(it);
    }
    _readBuffers.erase(clientId);
    _writeBuffers.erase(clientId);
    _clients.erase(clientId);
    _eventQueue.push({.type = EventType::CLIENT_DISCONNECTED, .clientId = clientId, .message = ""});
}

void SessionManager::handleServerEvent(const short revents) {
    if ((revents & POLLIN) != 0) {
        acceptNewConnection();
    }
}

void SessionManager::handleClientEvent(const struct pollfd pfd) {
    const int clientId = pfd.fd;

    if ((pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) != 0) {
        disconnectClient(clientId);
        return;
    }
    if ((pfd.revents & POLLIN) != 0) {
        handleClientRead(clientId);
        if (!_clients.contains(clientId)) {
            return;
        }
    }
    if ((pfd.revents & POLLOUT) != 0) {
        handleClientWrite(clientId);
    }
}

void SessionManager::acceptNewConnection() {
    std::optional<socket::ClientSocket> newClientOpt = _serverSocket.acceptClient();
    if (!newClientOpt.has_value()) {
        return;
    }
    socket::ClientSocket newClient = std::move(newClientOpt.value());
    const int clientId = newClient.fd();

    _clients.emplace(clientId, std::move(newClient));
    _pollFds.push_back({.fd = clientId, .events = POLLIN, .revents = 0});
    _eventQueue.push({.type = EventType::CLIENT_CONNECTED, .clientId = clientId, .message = ""});
}

void SessionManager::handleClientRead(const int clientId) {
    try {
        const std::string data = _clients.at(clientId).receive();

        if (data.empty()) {
            return;
        }
        _readBuffers[clientId] += data;
        if (_readBuffers[clientId].size() > socket::kMaxBufferSize) {
            throw exception::SocketError{"read buffer overflow, disconnecting client"};
        }
        extractCompleteMessages(clientId);

    } catch (const std::exception& /*e*/) {
        disconnectClient(clientId);
    }
}

void SessionManager::handleClientWrite(const int clientId) {
    if (!_clients.contains(clientId) || !_writeBuffers.contains(clientId)) {
        return;
    }
    std::string& buffer = _writeBuffers.at(clientId);

    if (buffer.empty()) {
        return;
    }
    try {
        const std::size_t bytesSent = _clients.at(clientId).send(buffer);

        if (bytesSent > 0) {
            buffer.erase(0, bytesSent);
        }
        if (buffer.empty()) {
            for (auto& pfd : _pollFds) {
                if (pfd.fd == clientId) {
                    pfd.events &= ~POLLOUT;
                    break;
                }
            }
        }
    } catch (const std::exception&) {
        disconnectClient(clientId);
    }
}

void SessionManager::extractCompleteMessages(const int clientId) {
    std::string& buffer = _readBuffers.at(clientId);

    std::size_t pos = 0;
    while ((pos = buffer.find('\n')) != std::string::npos) {
        std::string completeMessage = buffer.substr(0, pos);
        _eventQueue.push(NetworkEvent{
            .type = EventType::MESSAGE_RECEIVED, .clientId = clientId, .message = std::move(completeMessage)});
        buffer.erase(0, pos + 1);
    }
}

}  // namespace network
