/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** MockClientSocket
*/

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

#include "network/IClientSocket.hpp"

namespace zappy::gui::network::tests {

class MockClientSocket : public shared::network::IClientSocket {
  public:
    MOCK_METHOD(std::size_t, send, (std::string_view message), (const, override));
    MOCK_METHOD(std::string, receive, (), (const, override));
    MOCK_METHOD(std::optional<std::string>, tryPopMessage, (), (override));
};

}  // namespace zappy::gui::network::tests
