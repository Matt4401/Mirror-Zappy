/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Client
*/

#pragma once

#include <functional>
#include <memory>

#include "EventDispatcher.hpp"
#include "network/IClientSocket.hpp"
#include "strategy/GUIStrategy.hpp"

namespace zappy::gui::network {
class Client {
  public:
    static constexpr auto DefaultTeamName = "GRAPHIC\n";

    explicit Client(const parser::GuiConfig& config, events::EventDispatcher& dispatcher);
    explicit Client(std::unique_ptr<shared::network::IClientSocket> socket, events::EventDispatcher& dispatcher);
    ~Client();
    Client(const Client& other) = delete;
    Client& operator=(const Client& other) = delete;
    Client(Client&& other) = delete;
    Client& operator=(Client&& other) = delete;

    [[nodiscard]] shared::network::IClientSocket& socket() const { return *_socket; }

    /**
     * @brief Poll the socket and parse incoming server messages.
     * Temporary example to be moved to a dedicated system.
     */
    void update();

  private:
    void initNetwork();

    std::unique_ptr<shared::network::IClientSocket> _socket;
    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    events::EventDispatcher::EventToken _sendToken{0};
};
}  // namespace zappy::gui::network
