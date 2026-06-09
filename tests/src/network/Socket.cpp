/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** socket tests
*/

#include <fcntl.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
#include <string>

#include "exception/SocketError.hpp"
#include "network/ClientSocket.hpp"
#include "network/ServerSocket.hpp"

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
    const int getSocknameResult =
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        ::getsockname(server.fd(), reinterpret_cast<sockaddr*>(&boundAddress), &addressLength);
    ASSERT_NE(getSocknameResult, -1);

    _mockClientFd = ::socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(_mockClientFd, -1);

    boundAddress.sin_family = AF_INET;
    boundAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    const auto connectResult =
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        ::connect(_mockClientFd, reinterpret_cast<const sockaddr*>(&boundAddress), sizeof(boundAddress));
    ASSERT_NE(connectResult, -1);

    const shared::network::ClientSocket acceptedClient = server.acceptClient();

    EXPECT_NE(acceptedClient.fd(), -1);
}

TEST_F(ServerSocketTest, AcceptClientOnNonBlockingWithNoConnectionsReturnsInvalidSocket) {
    server::network::ServerSocket server;
    server.bindAndListen(0);

    const int flags = ::fcntl(server.fd(), F_GETFL, 0);
    ASSERT_NE(flags, -1);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    ::fcntl(server.fd(), F_SETFL, flags | O_NONBLOCK);

    const shared::network::ClientSocket phantomClient = server.acceptClient();

    EXPECT_THROW({ [[maybe_unused]] const int fd = phantomClient.fd(); }, shared::exception::SocketError);
}

TEST(NetworkSocketTest, ClientConnectsToServerSuccessfully) {
    server::network::ServerSocket server{};
    server.bindAndListen(0);

    sockaddr_in serverAddress{};
    socklen_t length = sizeof(serverAddress);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    ASSERT_EQ(::getsockname(server.fd(), reinterpret_cast<sockaddr*>(&serverAddress), &length), 0);
    const std::uint16_t port = ::ntohs(serverAddress.sin_port);

    shared::network::ClientSocket client{};
    EXPECT_NO_THROW(client.connectToServer("127.0.0.1", port));

    const shared::network::ClientSocket serverSideClient = server.acceptClient();
    EXPECT_GE(serverSideClient.fd(), 0);
}

TEST(NetworkSocketTest, ClientAndServerExchangeData) {
    server::network::ServerSocket server{};
    server.bindAndListen(0);

    sockaddr_in serverAddress{};
    socklen_t length = sizeof(serverAddress);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    ASSERT_EQ(::getsockname(server.fd(), reinterpret_cast<sockaddr*>(&serverAddress), &length), 0);
    const std::uint16_t port = ::ntohs(serverAddress.sin_port);

    shared::network::ClientSocket client{};
    client.connectToServer("127.0.0.1", port);
    const shared::network::ClientSocket serverSideClient = server.acceptClient();

    const std::string expectedMessage = "WELCOME\n";
    EXPECT_EQ(serverSideClient.send(expectedMessage), expectedMessage.size());

    const std::string receivedMessage = client.receive();
    EXPECT_EQ(receivedMessage, expectedMessage);
}

}  // namespace zappy::network::tests
