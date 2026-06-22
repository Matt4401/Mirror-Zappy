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

using ::testing::Return;
using ::testing::Sequence;
using ::testing::StrictMock;

namespace zappy::gui::network::tests {

namespace {
class ClientTest : public ::testing::Test {
  protected:
    void SetUp() override {
        _dispatcher = std::make_shared<events::EventDispatcher>();
        _mockSocket = std::make_unique<StrictMock<MockClientSocket>>();
        _mockSocketPtr = _mockSocket.get();
    }
    [[nodiscard]] std::shared_ptr<events::EventDispatcher>& getDispatcher() { return _dispatcher; }
    [[nodiscard]] std::unique_ptr<StrictMock<MockClientSocket>>& getMockSocket() { return _mockSocket; }
    [[nodiscard]] StrictMock<MockClientSocket>* getMockSocketPtr() const { return _mockSocketPtr; }

  private:
    std::shared_ptr<events::EventDispatcher> _dispatcher;
    std::unique_ptr<StrictMock<MockClientSocket>> _mockSocket;
    StrictMock<MockClientSocket>* _mockSocketPtr{nullptr};
};
}  // namespace

TEST_F(ClientTest, Constructor_Success) {
    Sequence const s;

    EXPECT_CALL(*getMockSocketPtr(), tryPopMessage())
        .InSequence(s)
        .WillOnce(Return(std::optional<std::string>{"WELCOME"}));

    EXPECT_CALL(*getMockSocketPtr(), send(std::string_view("GRAPHIC\n"))).InSequence(s).WillOnce(Return(8));

    EXPECT_NO_THROW({ const Client client(std::move(getMockSocket()), getDispatcher()); });
}

TEST_F(ClientTest, Constructor_Timeout) {
    EXPECT_CALL(*getMockSocketPtr(), tryPopMessage()).WillRepeatedly(Return(std::nullopt));

    EXPECT_THROW(
        { const Client client(std::move(getMockSocket()), getDispatcher()); }, ::network::exception::SocketError);
}

TEST_F(ClientTest, Constructor_BadWelcome) {
    EXPECT_CALL(*getMockSocketPtr(), tryPopMessage()).WillOnce(Return(std::optional<std::string>{"BAD_WELCOME"}));

    EXPECT_THROW(
        { const Client client(std::move(getMockSocket()), getDispatcher()); }, ::network::exception::SocketError);
}

TEST_F(ClientTest, SendCommand_EventTrigger) {
    Sequence s;

    EXPECT_CALL(*getMockSocketPtr(), tryPopMessage())
        .InSequence(s)
        .WillOnce(Return(std::optional<std::string>{"WELCOME"}));

    EXPECT_CALL(*getMockSocketPtr(), send(std::string_view("GRAPHIC\n"))).InSequence(s).WillOnce(Return(8));

    const Client client(std::move(getMockSocket()), getDispatcher());

    EXPECT_CALL(*getMockSocketPtr(), send(std::string_view("msz\n"))).InSequence(s).WillOnce(Return(4));

    getDispatcher()->dispatch(events::SendCommand{"msz\n"});
}

TEST_F(ClientTest, Update_ParsesServerMessage) {
    Sequence s;

    EXPECT_CALL(*getMockSocketPtr(), tryPopMessage())
        .InSequence(s)
        .WillOnce(Return(std::optional<std::string>{"WELCOME"}));

    EXPECT_CALL(*getMockSocketPtr(), send(std::string_view("GRAPHIC\n"))).InSequence(s).WillOnce(Return(8));

    Client client(std::move(getMockSocket()), getDispatcher());

    EXPECT_CALL(*getMockSocketPtr(), tryPopMessage())
        .InSequence(s)
        .WillOnce(Return(std::optional<std::string>{"msz 10 10\n"}))
        .WillOnce(Return(std::nullopt));

    bool received = false;
    auto token = getDispatcher()->subscribe<shared::protocol::server::Msz>(
        [&received](const shared::protocol::server::Msz& cmd) {
            EXPECT_EQ(cmd.width, 10);
            EXPECT_EQ(cmd.height, 10);
            received = true;
        });

    client.update();

    EXPECT_TRUE(received);
    getDispatcher()->unsubscribe<shared::protocol::server::Msz>(token);
}

}  // namespace zappy::gui::network::tests
