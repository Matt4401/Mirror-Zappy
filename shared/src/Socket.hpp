/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Socket class
*/

#include <cstddef>
#include <string>
#include <string_view>

namespace zappy::shared {

class Socket {
  public:
    Socket() = default;
    explicit Socket(int fd);
    ~Socket();

    Socket(const Socket& other) = delete;
    Socket& operator=(const Socket& other) = delete;
    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    [[nodiscard]] std::size_t send(const std::string_view& message) const;
    [[nodiscard]] std::string receive() const;

    [[nodiscard]] int fd() const;

  private:
    int _fd{-1};
};
}  // namespace zappy::shared
