/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Client
*/

#pragma once
#include <memory>

#include "cli/Parser.hpp"
#include "network/IClientSocket.hpp"

namespace zappy::gui::network {
class Client {
  public:
    static constexpr const char* DefaultTeamName = "GRAPHIC";

    explicit Client(const cli::Parser::Config& config);
    ~Client() = default;
    Client(const Client& other) = delete;
    Client& operator=(const Client& other) = delete;
    Client(Client&& other) = delete;
    Client& operator=(Client&& other) = delete;

    [[nodiscard]] shared::network::IClientSocket& socket() const { return *_socket; }

  protected:
  private:
    std::unique_ptr<shared::network::IClientSocket> _socket;
};
}  // namespace zappy::gui::network
