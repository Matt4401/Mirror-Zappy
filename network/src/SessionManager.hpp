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

#include "network/ISessionManager.hpp"
#include "socket/ClientSocket.hpp"
#include "socket/ServerSocket.hpp"

namespace network {

class SessionManager : public shared::network::ISessionManager {
  public:
    explicit SessionManager(std::uint16_t port);

    void pollNetwork() override;
    [[nodiscard]] bool tryPopMessage(NetworkEvent& message) override;
    void sendMessage(int clientId, std::string_view message) override;

    void disconnectClient(int clientId) override;

  private:
    void acceptNewConnection();
    void handleClientRead(int clientId);
    void handleClientWrite(int clientId);
    void extractCompleteMessages(int clientId);
    void handleServerEvent(short revents);
    void handleClientEvent(struct pollfd pfd);

    socket::ServerSocket _serverSocket;
    std::vector<struct pollfd> _pollFds;

    std::unordered_map<int, socket::ClientSocket> _clients;

    std::unordered_map<int, std::string> _readBuffers;
    std::unordered_map<int, std::string> _writeBuffers;
    std::queue<NetworkEvent> _eventQueue;
};

}  // namespace network
