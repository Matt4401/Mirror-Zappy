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
#include <optional>
#include <string>

#include "network/ClientSocket.hpp"
#include "network/ServerSocket.hpp"

namespace zappy::server::network {

TEST(ServerSocketTest, AcceptClientOnNonBlockingWithNoConnectionsReturnsInvalidSocket) {
    const network::ServerSocket server{0};

    const int flags = ::fcntl(server.fd(), F_GETFL, 0);
    ASSERT_NE(flags, -1);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    ::fcntl(server.fd(), F_SETFL, flags | O_NONBLOCK);
    const std::optional<shared::network::ClientSocket> phantomClient = server.acceptClient();
    EXPECT_FALSE(phantomClient.has_value());
}

TEST(NetworkSocketTest, ClientConnectsToServerSuccessfully) {
    const network::ServerSocket server{0};

    sockaddr_in serverAddress{};
    socklen_t length = sizeof(serverAddress);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    ASSERT_EQ(::getsockname(server.fd(), reinterpret_cast<sockaddr*>(&serverAddress), &length), 0);
    const std::uint16_t port = ::ntohs(serverAddress.sin_port);

    const shared::network::ClientSocket client{"127.0.0.1", port};

    const std::optional<shared::network::ClientSocket> serverSideClientOpt = server.acceptClient();
    if (serverSideClientOpt.has_value()) {
        ASSERT_TRUE(serverSideClientOpt.has_value());
    }
    EXPECT_GE(serverSideClientOpt.value().fd(), 0);
}

TEST(NetworkSocketTest, ClientAndServerExchangeData) {
    const network::ServerSocket server{0};

    sockaddr_in serverAddress{};
    socklen_t length = sizeof(serverAddress);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    ASSERT_EQ(::getsockname(server.fd(), reinterpret_cast<sockaddr*>(&serverAddress), &length), 0);
    const std::uint16_t port = ::ntohs(serverAddress.sin_port);

    const shared::network::ClientSocket client{"127.0.0.1", port};

    const std::optional<shared::network::ClientSocket> serverSideClientOpt = server.acceptClient();
    ASSERT_TRUE(serverSideClientOpt.has_value());

    const std::string expectedMessage = "WELCOME\n";
    if (serverSideClientOpt.has_value()) {
        EXPECT_EQ(serverSideClientOpt.value().send(expectedMessage), expectedMessage.size());
    }
    const std::string receivedMessage = client.receive();
    EXPECT_EQ(receivedMessage, expectedMessage);
}
}  // namespace zappy::server::network
