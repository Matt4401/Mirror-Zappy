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

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <thread>

#include "socket/ClientSocket.hpp"
#include "socket/ServerSocket.hpp"

namespace {

std::uint16_t getServerPort(const network::socket::ServerSocket& server) {
    sockaddr_in serverAddress{};
    socklen_t length = sizeof(serverAddress);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    ::getsockname(server.fd(), reinterpret_cast<sockaddr*>(&serverAddress), &length);
    return ::ntohs(serverAddress.sin_port);
}

}  // namespace

namespace network::socket {

TEST(ServerSocketTest, AcceptClientOnNonBlockingWithNoConnectionsReturnsInvalidSocket) {
    const ServerSocket server{0};

    const int flags = ::fcntl(server.fd(), F_GETFL, 0);
    ASSERT_NE(flags, -1);

    EXPECT_NE(flags & O_NONBLOCK, 0);
    const std::optional<ClientSocket> phantomClient = server.acceptClient();
    EXPECT_FALSE(phantomClient.has_value());
}

TEST(NetworkSocketTest, ClientConnectsToServerSuccessfully) {
    const ServerSocket server{0};

    sockaddr_in serverAddress{};
    socklen_t length = sizeof(serverAddress);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    ASSERT_EQ(::getsockname(server.fd(), reinterpret_cast<sockaddr*>(&serverAddress), &length), 0);
    const std::uint16_t port = ::ntohs(serverAddress.sin_port);

    const ClientSocket client{"127.0.0.1", port};

    const std::optional<ClientSocket> serverSideClientOpt = server.acceptClient();
    if (serverSideClientOpt.has_value()) {
        ASSERT_TRUE(serverSideClientOpt.has_value());
    }
    if (serverSideClientOpt.has_value()) {
        EXPECT_GE(serverSideClientOpt.value().fd(), 0);
    }
}

TEST(NetworkSocketTest, ClientAndServerExchangeData) {
    const ServerSocket server{0};

    sockaddr_in serverAddress{};
    socklen_t length = sizeof(serverAddress);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    ASSERT_EQ(::getsockname(server.fd(), reinterpret_cast<sockaddr*>(&serverAddress), &length), 0);
    const std::uint16_t port = ::ntohs(serverAddress.sin_port);

    const ClientSocket client{"127.0.0.1", port};

    const std::optional<ClientSocket> serverSideClientOpt = server.acceptClient();
    ASSERT_TRUE(serverSideClientOpt.has_value());

    const std::string expectedMessage = "WELCOME\n";
    if (serverSideClientOpt.has_value()) {
        EXPECT_EQ(serverSideClientOpt.value().send(expectedMessage), expectedMessage.size());
    }
    const std::string receivedMessage = client.receive();
    EXPECT_EQ(receivedMessage, expectedMessage);
}

TEST(ClientSocketBufferTest, TryPopMessageExtractsCompleteMessage) {
    const ServerSocket server{0};
    ClientSocket client{"127.0.0.1", getServerPort(server)};

    std::optional<ClientSocket> serverSideOpt = server.acceptClient();
    ASSERT_TRUE(serverSideOpt.has_value());

    client.setNonBlocking();

    if (serverSideOpt.value().send("Forward\n") != 8) {
        FAIL() << "Failed to send complete message to client";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::optional<std::string> msg = client.tryPopMessage();
    ASSERT_TRUE(msg.has_value());
    EXPECT_EQ(msg.value(), "Forward");

    EXPECT_FALSE(client.tryPopMessage().has_value());
}

}  // namespace network::socket
