/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Socket class
*/

#include <string>
#include <string_view>
namespace zappy::shared {
class Socket {
  public:
    Socket() = default;
    ~Socket() = default;

    Socket(const Socket& other) = delete;
    Socket& operator=(const Socket& other) = delete;
    Socket(Socket&& other) noexcept = default;
    Socket& operator=(Socket&& other) noexcept = default;

    void send(const std::string_view& message) const;
    std::string receive();

    [[nodiscard]] int fd() const;

  private:
    int _fd{-1};
};
}  // namespace zappy::shared
