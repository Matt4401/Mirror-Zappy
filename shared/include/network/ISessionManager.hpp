/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** session manager interface
*/

#pragma once

#include <cstdint>
#include <string>
#include <string_view>
namespace zappy::shared::network {

class ISessionManager {
  public:
    enum class EventType : std::uint8_t { CLIENT_CONNECTED, CLIENT_DISCONNECTED, MESSAGE_RECEIVED };

    struct NetworkEvent {
        EventType type;
        int clientId;
        std::string message;
    };

    ISessionManager() = default;
    ISessionManager(const ISessionManager&) = delete;
    ISessionManager& operator=(const ISessionManager&) = delete;
    ISessionManager(ISessionManager&&) = delete;
    ISessionManager& operator=(ISessionManager&&) = delete;
    virtual ~ISessionManager() = default;

    virtual void pollNetwork(int timeout) = 0;

    [[nodiscard]] virtual bool tryPopMessage(NetworkEvent& event) = 0;
    virtual void sendMessage(int clientId, std::string_view message) = 0;
    virtual void disconnectClient(int clientId) = 0;

    [[nodiscard]] virtual bool isWriting(int clientId) const = 0;
};

}  // namespace zappy::shared::network
