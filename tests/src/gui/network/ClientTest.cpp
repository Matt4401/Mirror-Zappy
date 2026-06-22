/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** ClientTest
*/

#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "EventDispatcher.hpp"
#include "MockClientSocket.hpp"
#include "events/GuiEvents.hpp"
#include "exception/SocketError.hpp"
#include "gmock/gmock.h"
#include "network/Client.hpp"
#include "protocol/Commands.hpp"

using ::testing::_;
using ::testing::Return;
using ::testing::Sequence;
using ::testing::StrictMock;

namespace zappy::gui::network::tests {

class ClientTest : public ::testing::Test {
  protected:
    void SetUp() override {
        _dispatcher = std::make_shared<events::EventDispatcher>();
        _mockSocket = std::make_unique<StrictMock<MockClientSocket>>();
        _mockSocketPtr = _mockSocket.get();
    }

    std::shared_ptr<events::EventDispatcher> _dispatcher;
    std::unique_ptr<StrictMock<MockClientSocket>> _mockSocket;
    StrictMock<MockClientSocket>* _mockSocketPtr{nullptr};
};

TEST_F(ClientTest, Constructor_Success) {
    Sequence s;

    EXPECT_CALL(*_mockSocketPtr, tryPopMessage()).InSequence(s).WillOnce(Return(std::optional<std::string>{"WELCOME"}));

    EXPECT_CALL(*_mockSocketPtr, send(std::string_view("GRAPHIC\n"))).InSequence(s).WillOnce(Return(8));

    EXPECT_NO_THROW({ Client client(std::move(_mockSocket), _dispatcher); });
}

TEST_F(ClientTest, Constructor_Timeout) {
    EXPECT_CALL(*_mockSocketPtr, tryPopMessage()).WillRepeatedly(Return(std::nullopt));

    EXPECT_THROW({ Client client(std::move(_mockSocket), _dispatcher); }, ::network::exception::SocketError);
}

TEST_F(ClientTest, Constructor_BadWelcome) {
    EXPECT_CALL(*_mockSocketPtr, tryPopMessage()).WillOnce(Return(std::optional<std::string>{"BAD_WELCOME"}));

    EXPECT_THROW({ Client client(std::move(_mockSocket), _dispatcher); }, ::network::exception::SocketError);
}

TEST_F(ClientTest, SendCommand_EventTrigger) {
    Sequence s;

    EXPECT_CALL(*_mockSocketPtr, tryPopMessage()).InSequence(s).WillOnce(Return(std::optional<std::string>{"WELCOME"}));

    EXPECT_CALL(*_mockSocketPtr, send(std::string_view("GRAPHIC\n"))).InSequence(s).WillOnce(Return(8));

    Client client(std::move(_mockSocket), _dispatcher);

    EXPECT_CALL(*_mockSocketPtr, send(std::string_view("msz\n"))).InSequence(s).WillOnce(Return(4));

    _dispatcher->dispatch(events::SendCommand{"msz\n"});
}

TEST_F(ClientTest, Update_ParsesServerMessage) {
    Sequence s;

    EXPECT_CALL(*_mockSocketPtr, tryPopMessage()).InSequence(s).WillOnce(Return(std::optional<std::string>{"WELCOME"}));

    EXPECT_CALL(*_mockSocketPtr, send(std::string_view("GRAPHIC\n"))).InSequence(s).WillOnce(Return(8));

    Client client(std::move(_mockSocket), _dispatcher);

    EXPECT_CALL(*_mockSocketPtr, tryPopMessage())
        .InSequence(s)
        .WillOnce(Return(std::optional<std::string>{"msz 10 10\n"}))
        .WillOnce(Return(std::nullopt));

    bool received = false;
    auto token =
        _dispatcher->subscribe<shared::protocol::server::Msz>([&received](const shared::protocol::server::Msz& cmd) {
            EXPECT_EQ(cmd.width, 10);
            EXPECT_EQ(cmd.height, 10);
            received = true;
        });

    client.update();

    EXPECT_TRUE(received);
    _dispatcher->unsubscribe<shared::protocol::server::Msz>(token);
}

}  // namespace zappy::gui::network::tests
