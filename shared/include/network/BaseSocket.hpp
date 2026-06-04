/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** socket bases
*/

#pragma once

namespace zappy::shared::network {

class BaseSocket {
  public:
    virtual ~BaseSocket();

    BaseSocket(const BaseSocket&) = delete;
    BaseSocket& operator=(const BaseSocket&) = delete;

    BaseSocket(BaseSocket&& other) noexcept;
    BaseSocket& operator=(BaseSocket&& other) noexcept;

    [[nodiscard]] int fd() const;
    void close();

  protected:
    BaseSocket() = default;
    explicit BaseSocket(int fd);
    void setFd(int fd);

  private:
    int _fd{-1};
};

}  // namespace zappy::shared::network
