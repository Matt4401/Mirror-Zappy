/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** protocol parser interface
*/

#pragma once

#include <functional>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

#include "protocol/Commands.hpp"

namespace zappy::shared::protocol {

class Parser {
  public:
    Parser();
    ~Parser() = default;

    Parser(const Parser& other) = delete;
    Parser& operator=(const Parser& other) = delete;
    Parser(Parser&& other) = delete;
    Parser& operator=(Parser&& other) = delete;

    /**
     * @brief Parse a raw protocol string sent by the Server to the GUI.
     */
    [[nodiscard]] ServerCommand parseServerCommand(std::string_view input) const;

    /**
     * @brief Parse a raw protocol string sent by the GUI to the Server.
     */
    [[nodiscard]] ClientCommand parseClientCommand(std::string_view input) const;

  private:
    using ServerParseFunc = std::function<ServerCommand(std::istringstream&)>;
    using ClientParseFunc = std::function<ClientCommand(std::istringstream&)>;

    std::unordered_map<std::string, ServerParseFunc> _serverDispatch;
    std::unordered_map<std::string, ClientParseFunc> _clientDispatch;

    void initServerDispatch();
    void initClientDispatch();
};

}  // namespace zappy::shared::protocol
