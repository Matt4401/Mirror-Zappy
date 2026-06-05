/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Session manager tests
*/

#include "network/SessionManager.hpp"

#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>
#include <thread>

namespace zappy::server::network::tests {

namespace {
class SessionManagerTest : public ::testing::Test {
  protected:
    static constexpr std::uint16_t TEST_PORT = 4242;

    void SetUp() override {
        _mockClientFd = ::socket(AF_INET, SOCK_STREAM, 0);
        ASSERT_NE(_mockClientFd, -1);
    }

    void TearDown() override {
        if (_mockClientFd != -1) {
            ::close(_mockClientFd);
        }
    }

    void connectMockClient() const {
        sockaddr_in serverAddress{};
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(TEST_PORT);
        serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        const int result =
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            ::connect(_mockClientFd, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress));
        ASSERT_NE(result, -1) << "Mock client failed to connect to SessionManager";
    }

    // NOLINTNEXTLINE (cppcoreguidelines-non-private-member-variables-in-classes)
    int _mockClientFd{-1};
};

}  // namespace

TEST_F(SessionManagerTest, AcceptsNewConnectionAndGeneratesEvent) {
    SessionManager manager{TEST_PORT};

    connectMockClient();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    manager.pollNetwork();

    SessionManager::NetworkEvent event;
    const bool hasEvent = manager.tryPopMessage(event);

    ASSERT_TRUE(hasEvent);
    EXPECT_EQ(event.type, SessionManager::EventType::CLIENT_CONNECTED);
    EXPECT_GT(event.clientId, 0);
}

TEST_F(SessionManagerTest, ReceivesCompleteMessageAndGeneratesEvent) {
    SessionManager manager{TEST_PORT};
    connectMockClient();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    manager.pollNetwork();
    SessionManager::NetworkEvent event;
    auto received = manager.tryPopMessage(event);
    ASSERT_TRUE(received);

    const std::string command = "Forward\n";
    ::send(_mockClientFd, command.data(), command.size(), MSG_NOSIGNAL);
    const ::ssize_t bytesSent = ::send(_mockClientFd, command.data(), command.size(), MSG_NOSIGNAL);
    ASSERT_EQ(bytesSent, static_cast<::ssize_t>(command.size()));

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    manager.pollNetwork();

    const bool hasEvent = manager.tryPopMessage(event);

    ASSERT_TRUE(hasEvent);
    EXPECT_EQ(event.type, SessionManager::EventType::MESSAGE_RECEIVED);
    EXPECT_EQ(event.message, "Forward");
}

TEST_F(SessionManagerTest, HandlesTCPFragmentationPerfectly) {
    SessionManager manager{TEST_PORT};
    connectMockClient();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    manager.pollNetwork();

    SessionManager::NetworkEvent event;
    const auto received = manager.tryPopMessage(event);
    ASSERT_TRUE(received);

    const std::string part1 = "For";
    const ::ssize_t bytesSent1 = ::send(_mockClientFd, part1.data(), part1.size(), MSG_NOSIGNAL);
    ASSERT_EQ(bytesSent1, static_cast<::ssize_t>(part1.size()));
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    manager.pollNetwork();

    EXPECT_FALSE(manager.tryPopMessage(event));
    const std::string part2 = "ward\n";
    const ::ssize_t bytesSent2 = ::send(_mockClientFd, part2.data(), part2.size(), MSG_NOSIGNAL);
    ASSERT_EQ(bytesSent2, static_cast<::ssize_t>(part2.size()));
    ::send(_mockClientFd, part2.data(), part2.size(), MSG_NOSIGNAL);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    manager.pollNetwork();
    const bool hasEvent = manager.tryPopMessage(event);
    ASSERT_TRUE(hasEvent);
    EXPECT_EQ(event.type, SessionManager::EventType::MESSAGE_RECEIVED);
    EXPECT_EQ(event.message, "Forward");
}

TEST_F(SessionManagerTest, SendsDataToClient) {
    SessionManager manager{TEST_PORT};
    connectMockClient();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    manager.pollNetwork();

    SessionManager::NetworkEvent event;
    auto received = manager.tryPopMessage(event);
    ASSERT_TRUE(received);
    const int clientId = event.clientId;

    const std::string response = "WELCOME\n";
    manager.sendMessage(clientId, response);
    manager.pollNetwork();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    std::array<char, 32> buffer{};
    const ::ssize_t bytesRead = ::recv(_mockClientFd, buffer.data(), buffer.size(), 0);

    ASSERT_GT(bytesRead, 0);
    EXPECT_EQ(std::string(buffer.data(), static_cast<std::size_t>(bytesRead)), response);
}

TEST_F(SessionManagerTest, HandlesClientDisconnection) {
    SessionManager manager{TEST_PORT};
    connectMockClient();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    manager.pollNetwork();

    SessionManager::NetworkEvent event;
    auto received = manager.tryPopMessage(event);
    ASSERT_TRUE(received);

    ::close(_mockClientFd);
    _mockClientFd = -1;
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    manager.pollNetwork();

    const bool hasEvent = manager.tryPopMessage(event);

    ASSERT_TRUE(hasEvent);
    EXPECT_EQ(event.type, SessionManager::EventType::CLIENT_DISCONNECTED);
}

}  // namespace zappy::server::network::tests
