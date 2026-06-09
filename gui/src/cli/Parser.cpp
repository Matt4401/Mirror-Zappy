/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Parser
*/

#include "Parser.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

#include "exception/ParserError.hpp"

namespace zappy::cli {
bool Parser::parse() {
    if (_args.size() == 1 && _args.front() == "--help") {
        std::cout << printHelp() << std::endl;
        return false;
    }
    if (_args.empty() || _args.size() < 4) {
        throw shared::exception::ParserError(printHelp());
    }
    parseArgs();
    return true;
}

std::string Parser::printHelp() { return "USAGE: ./zappy_gui -p port -h machine"; }

void Parser::parseArgs() {
    for (std::size_t i = 0; i < _args.size(); i += 2) {
        if (_args.at(i) == "-p") {
            if (i + 1 >= _args.size()) {
                throw shared::exception::ParserError("Port number is missing");
            }
            _port = getPort(_args.at(i + 1));
        } else if (_args.at(i) == "-h") {
            if (i + 1 >= _args.size()) {
                throw shared::exception::ParserError("Machine address is missing");
            }
            _machine = getMachine(_args.at(i + 1));
        } else {
            throw shared::exception::ParserError("Unknown argument: " + _args.at(i));
        }
    }
}
// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
std::uint16_t Parser::getPort(const std::string &value) {
    char *endPtr = nullptr;
    const long port = std::strtol(value.c_str(), &endPtr, 10);

    if (*endPtr != '\0' || port <= 0 || port > kPortMax) {
        throw shared::exception::ParserError("Invalid port: '" + value + "' must be between 1 and " +
                                             std::to_string(kPortMax));
    }
    return static_cast<std::uint16_t>(port);
}

std::string Parser::getMachine(const std::string &value) {
    return value;
}
// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)
}  // namespace zappy::cli
