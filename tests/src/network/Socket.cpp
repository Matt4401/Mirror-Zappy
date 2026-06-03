/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** socket tests
*/

#include <arpa/inet.h>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdexcept>

#include "network/ServerSocket.hpp"
#include "src/exception/SocketError.hpp"
#include "src/network/ClientSocket.hpp"

namespace {

class ServerSocketTest : public ::testing::Test {
  protected:
    void TearDown() override {
        if (_mockClientFd != -1) {
            ::close(_mockClientFd);
        }
    }

    // NOLINTNEXTLINE (cppcoreguidelines-non-private-member-variables-in-classes)
    int _mockClientFd{-1};
};

}  // namespace

namespace zappy::network::tests {

TEST_F(ServerSocketTest, BindAndListenSucceedsOnAvailablePort) {
    server::network::ServerSocket server;
    EXPECT_NO_THROW({ server.bindAndListen(0); });
    EXPECT_NE(server.fd(), -1);
}

TEST_F(ServerSocketTest, AcceptClientReturnsValidClientSocket) {
    server::network::ServerSocket server;
    server.bindAndListen(0);

    sockaddr_in boundAddress{};
    socklen_t addressLength{sizeof(boundAddress)};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    ::getsockname(server.fd(), reinterpret_cast<sockaddr*>(&boundAddress), &addressLength);

    _mockClientFd = ::socket(AF_INET, SOCK_STREAM, 0);
    boundAddress.sin_family = AF_INET;
    boundAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    auto i = ::connect(_mockClientFd, reinterpret_cast<const sockaddr*>(&boundAddress), sizeof(boundAddress));

    const shared::network::ClientSocket acceptedClient = server.acceptClient();

    EXPECT_NE(acceptedClient.fd(), -1);
}

TEST_F(ServerSocketTest, AcceptClientOnNonBlockingWithNoConnectionsReturnsInvalidSocket) {
    server::network::ServerSocket server;
    server.bindAndListen(0);

    const int flags = ::fcntl(server.fd(), F_GETFL, 0);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    ::fcntl(server.fd(), F_SETFL, flags | O_NONBLOCK);

    const shared::network::ClientSocket phantomClient = server.acceptClient();

    EXPECT_THROW({ [[maybe_unused]] const int fd = phantomClient.fd(); }, shared::exception::SocketError);
}

}  // namespace zappy::network::tests
