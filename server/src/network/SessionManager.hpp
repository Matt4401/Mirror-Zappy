/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Session manager
*/

#pragma once

namespace zappy::server::network {

class SessionManager {
  public:
    SessionManager() = default;
    ~SessionManager() = default;

    SessionManager(const SessionManager& other) = delete;
    SessionManager& operator=(const SessionManager& other) = delete;
    SessionManager(SessionManager&& other) noexcept = default;
    SessionManager& operator=(SessionManager&& other) noexcept = default;

  private:
};

}  // namespace zappy::server::network
