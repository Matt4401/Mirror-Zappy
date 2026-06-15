/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** protocol parser interface
*/

#pragma once

#include <string_view>

#include "protocol/Commands.hpp"

namespace zappy::shared::protocol {

class Parser {
  public:
    Parser() = default;
    ~Parser() = default;

    Parser(const Parser& other) = delete;
    Parser& operator=(const Parser& other) = delete;
    Parser(Parser&& other) = delete;
    Parser& operator=(Parser&& other) = delete;

    /**
     * @brief Parse a raw protocol string sent by the Server to the GUI.
     */
    [[nodiscard]] static ServerCommand parseServerCommand(std::string_view input);

    /**
     * @brief Parse a raw protocol string sent by the GUI to the Server.
     */
    [[nodiscard]] static ClientCommand parseClientCommand(std::string_view input);
};

}  // namespace zappy::shared::protocol
