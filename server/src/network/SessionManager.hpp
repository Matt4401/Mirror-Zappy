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
    enum class EventType : std::uint8_t { CLIENT_CONNECTED, CLIENT_DISCONNECTED, MESSAGE_RECEIVED };

    struct NetworkEvent {
        EventType type;
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
    [[nodiscard]] bool tryPopMessage(NetworkEvent& message);
    void sendMessage(int clientId, std::string_view message);

    void disconnectClient(int clientId);

  private:
    void acceptNewConnection();
    void handleClientRead(int clientId);
    void handleClientWrite(int clientId);
    void extractCompleteMessages(int clientId);
    void handleServerEvent(short revents);
    void handleClientEvent(struct pollfd pfd);

    ServerSocket _serverSocket{};
    std::vector<struct pollfd> _pollFds;

    std::unordered_map<int, shared::network::ClientSocket> _clients;

    std::unordered_map<int, std::string> _readBuffers;
    std::unordered_map<int, std::string> _writeBuffers;
    std::queue<NetworkEvent> _messageQueue;
};

}  // namespace zappy::server::network
