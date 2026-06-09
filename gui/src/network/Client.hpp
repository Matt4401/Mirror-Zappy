/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Client
*/

#pragma once
#include "cli/Parser.hpp"
#include "network/ClientSocket.hpp"

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

    [[nodiscard]] const shared::network::ClientSocket& socket() const { return _socket; }

  protected:
  private:
    shared::network::ClientSocket _socket;
};
}  // namespace zappy::gui::network
