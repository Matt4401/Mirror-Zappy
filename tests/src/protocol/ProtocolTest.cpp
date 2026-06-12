#include <gtest/gtest.h>

#include <variant>

#include "protocol/Emitter.hpp"
#include "protocol/Parser.hpp"

using namespace zappy::shared::protocol;

TEST(ProtocolTest, ParsesServerMsz) {
    Parser const parser;
    auto const cmd = parser.parseServerCommand("msz 15 25");

    ASSERT_TRUE(std::holds_alternative<server::Msz>(cmd));
    auto const& msz = std::get<server::Msz>(cmd);
    EXPECT_EQ(msz.width, 15);
    EXPECT_EQ(msz.height, 25);
}

TEST(ProtocolTest, ParsesClientMsz) {
    Parser const parser;
    auto const cmd = parser.parseClientCommand("msz");

    ASSERT_TRUE(std::holds_alternative<client::Msz>(cmd));
}

TEST(ProtocolTest, HandlesUnknownCommands) {
    Parser const parser;
    auto const serverCmd = parser.parseServerCommand("unknown_cmd 123");
    auto const clientCmd = parser.parseClientCommand("invalid_cmd");

    EXPECT_TRUE(std::holds_alternative<UnknownCommand>(serverCmd));
    EXPECT_TRUE(std::holds_alternative<UnknownCommand>(clientCmd));
}

TEST(ProtocolTest, EmitsCommands) {
    server::Msz const serverMsz{30, 40};
    EXPECT_EQ(Emitter::build(serverMsz), "msz 30 40\n");

    client::Msz const clientMsz{};
    EXPECT_EQ(Emitter::build(clientMsz), "msz\n");
}
