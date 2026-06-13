/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Client
*/

#pragma once

#include <memory>

#include "events/EventDispatcher.hpp"
#include "network/IClientSocket.hpp"
#include "strategy/GUIStrategy.hpp"

namespace zappy::gui::network {
class Client {
  public:
    static constexpr auto DefaultTeamName = "GRAPHIC\n";

    explicit Client(const parser::GuiConfig& config, std::shared_ptr<events::EventDispatcher> dispatcher);
    ~Client() = default;
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

  protected:
  private:
    std::unique_ptr<shared::network::IClientSocket> _socket;
    std::shared_ptr<events::EventDispatcher> _dispatcher;
};
}  // namespace zappy::gui::network
