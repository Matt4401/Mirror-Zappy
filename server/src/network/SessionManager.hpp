/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Session manager
*/

#pragma once

#include <poll.h>
#include <sys/poll.h>

#include <cstdint>
#include <queue>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "network/ClientSocket.hpp"
#include "network/ServerSocket.hpp"

namespace zappy::server::network {

class SessionManager {
  public:
    struct NetworkMessage {
        int clientId;
        std::string message;
    };

    explicit SessionManager(std::uint16_t port);
    ~SessionManager() = default;

    SessionManager(const SessionManager& other) = delete;
    SessionManager& operator=(const SessionManager& other) = delete;
    SessionManager(SessionManager&& other) = delete;
    SessionManager& operator=(SessionManager&& other) = delete;

    void pollNetwork();
    [[nodiscard]] bool tryPopMessage(NetworkMessage& message);
    void sendMessage(int clientId, std::string_view message);

    void disconnectClient(int clientId);

  private:
    ServerSocket _serverSocket{};
    std::vector<struct pollfd> _pollFds;

    std::unordered_map<int, shared::network::ClientSocket> _clients;

    std::unordered_map<int, std::string> _readBuffers;
    std::unordered_map<int, std::string> _writeBuffers;
    std::queue<NetworkMessage> _messageQueue;
};

}  // namespace zappy::server::network
