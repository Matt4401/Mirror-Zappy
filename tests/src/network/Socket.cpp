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

namespace zappy::server::network {

TEST(ServerSocketTest, BindAndListenSucceedsOnAvailablePort) {
    EXPECT_NO_THROW({ const network::ServerSocket server{0}; });
}

TEST(ServerSocketTest, AcceptClientReturnsValidClientSocket) {
    const network::ServerSocket server{0};

    sockaddr_in boundAddress{};
    socklen_t addressLength{sizeof(boundAddress)};

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    ASSERT_NE(::getsockname(server.fd(), reinterpret_cast<sockaddr*>(&boundAddress), &addressLength), -1);

    const int mockClientFd = ::socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(mockClientFd, -1);

    boundAddress.sin_family = AF_INET;
    boundAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    ASSERT_NE(::connect(mockClientFd, reinterpret_cast<const sockaddr*>(&boundAddress), sizeof(boundAddress)), -1);

    const shared::network::ClientSocket acceptedClient = server.acceptClient();
    EXPECT_GE(acceptedClient.fd(), 0);

    ::close(mockClientFd);
}

TEST(ServerSocketTest, AcceptClientOnNonBlockingWithNoConnectionsReturnsInvalidSocket) {
    const network::ServerSocket server{0};

    const int flags = ::fcntl(server.fd(), F_GETFL, 0);
    ASSERT_NE(flags, -1);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    ::fcntl(server.fd(), F_SETFL, flags | O_NONBLOCK);

    const shared::network::ClientSocket phantomClient = server.acceptClient();
    EXPECT_THROW({ [[maybe_unused]] const int fd = phantomClient.fd(); }, shared::exception::SocketError);
}

TEST(NetworkSocketTest, ClientConnectsToServerSuccessfully) {
    const network::ServerSocket server{0};

    sockaddr_in serverAddress{};
    socklen_t length = sizeof(serverAddress);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    ASSERT_EQ(::getsockname(server.fd(), reinterpret_cast<sockaddr*>(&serverAddress), &length), 0);
    const std::uint16_t port = ::ntohs(serverAddress.sin_port);

    const shared::network::ClientSocket client{"127.0.0.1", port};

    const shared::network::ClientSocket serverSideClient = server.acceptClient();
    EXPECT_GE(serverSideClient.fd(), 0);
}

TEST(NetworkSocketTest, ClientAndServerExchangeData) {
    const network::ServerSocket server{0};

    sockaddr_in serverAddress{};
    socklen_t length = sizeof(serverAddress);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    ASSERT_EQ(::getsockname(server.fd(), reinterpret_cast<sockaddr*>(&serverAddress), &length), 0);
    const std::uint16_t port = ::ntohs(serverAddress.sin_port);

    const shared::network::ClientSocket client{"127.0.0.1", port};
    const shared::network::ClientSocket serverSideClient = server.acceptClient();

    const std::string expectedMessage = "WELCOME\n";
    EXPECT_EQ(serverSideClient.send(expectedMessage), expectedMessage.size());

    const std::string receivedMessage = client.receive();
    EXPECT_EQ(receivedMessage, expectedMessage);
}
}  // namespace zappy::server::network
