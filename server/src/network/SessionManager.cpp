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
#include <string>
#include <utility>

#include "exception/PollError.hpp"
#include "exception/SocketError.hpp"
#include "network/ClientSocket.hpp"

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
    shared::network::ClientSocket newClient = _serverSocket.acceptClient();
    const int clientId = newClient.fd();

    if (clientId == -1) {
        return;
    }
    _clients.emplace(clientId, std::move(newClient));
    _pollFds.push_back({.fd = clientId, .events = POLLIN | POLLOUT, .revents = 0});
    _eventQueue.push({.type = EventType::CLIENT_CONNECTED, .clientId = clientId, .message = ""});
}

void SessionManager::disconnectClient(const int clientId) {
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

void SessionManager::handleClientRead(const int clientId) {
    try {
        const std::string data = _clients.at(clientId).receive();

        if (data.empty()) {
            return;
        }
        _readBuffers.at(clientId) += data;
        extractCompleteMessages(clientId);

    } catch (const std::exception& /*e*/) {
        disconnectClient(clientId);
    }
}

void SessionManager::extractCompleteMessages(const int clientId) {
    std::string& buffer = _readBuffers.at(clientId);

    std::size_t pos = 0;
    while ((pos = buffer.find('\n')) != std::string::npos) {
        const std::string completeMessage = buffer.substr(0, pos);
        _eventQueue.push(
            NetworkEvent{.type = EventType::MESSAGE_RECEIVED, .clientId = clientId, .message = completeMessage});
        buffer.erase(0, pos + 1);
    }
}

}  // namespace zappy::server::network
