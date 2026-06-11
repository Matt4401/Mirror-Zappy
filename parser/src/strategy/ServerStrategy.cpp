/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ServerStrategy
*/

#include "strategy/ServerStrategy.hpp"

#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <unordered_map>

#include "Parser.hpp"
#include "encapsulation/GetoptWrapper.hpp"
#include "exception/ParsingError.hpp"

namespace zappy::parser {
void ServerStrategy::parse(const int argc, char** argv, ServerConfig& config) {
    if (handleUsage(argv, argc)) {
        throw shared::exception::ParsingError(kUsageThrowMessage);
    }
    processOptions(argc, argv, config);
    validate(config);
}

int ServerStrategy::parseNumericArg(const std::string& arg, const std::string& flagName) {
    try {
        const int val = static_cast<int>(std::stol(arg));

        if (val <= 0) {
            throw shared::exception::ParsingError("Invalid value for " + flagName + ": must be greater than 0.");
        }
        return val;
    } catch (const std::exception&) {
        throw shared::exception::ParsingError("Invalid formatting for " + flagName + ": '" + arg +
                                              "' is not a valid number.");
    }
}

void ServerStrategy::processOptions(const int argc, char** argv, ServerConfig& config) {
    const encapsulation::GetOptWrapper optionParser(argc, argv, kServerConfigFlags);
    const std::unordered_map<char, std::function<void(const std::string&)>> argumentHandlers = {
        {'p', [&config](const std::string& arg) { config.port = parseNumericArg(arg, "-p"); }},
        {'x', [&config](const std::string& arg) { config.width = parseNumericArg(arg, "-x"); }},
        {'y', [&config](const std::string& arg) { config.height = parseNumericArg(arg, "-y"); }},
        {'c', [&config](const std::string& arg) { config.clientLimit = parseNumericArg(arg, "-c"); }},
        {'f', [&config](const std::string& arg) { config.freq = parseNumericArg(arg, "-f"); }},
    };
    int opt = 0;

    while ((opt = optionParser.getNextOption()) != -1) {
        if (opt == 'n') {
            config.teamNames = optionParser.getMultiArgs();
            continue;
        }
        if (auto it = argumentHandlers.find(static_cast<char>(opt)); it != argumentHandlers.end()) {
            it->second(encapsulation::GetOptWrapper::getOptionArg());
        } else {
            throw shared::exception::ParsingError("Unknown option or missing argument near '-" +
                                                  std::string(1, encapsulation::GetOptWrapper::getUnknownOption()) +
                                                  "'");
        }
    }
}

void ServerStrategy::validate(const ServerConfig& config) {
    if (config.port <= 0) {
        throw shared::exception::ParsingError("Missing or invalid port (-p)");
    }
    if (config.width <= 0) {
        throw shared::exception::ParsingError("Missing or invalid width (-x)");
    }
    if (config.height <= 0) {
        throw shared::exception::ParsingError("Missing or invalid height (-y)");
    }
    if (config.clientLimit <= 0) {
        throw shared::exception::ParsingError("Missing or invalid clientsNb (-c)");
    }
    if (config.teamNames.empty()) {
        throw shared::exception::ParsingError("Server requires at least one team (-n)");
    }
    for (const auto& name : config.teamNames) {
        if (name == kReservedGraphicName) {
            throw shared::exception::ParsingError("Team name 'GRAPHIC' is reserved for the GUI.");
        }
    }
}

bool ServerStrategy::handleUsage(char** argv, const int argc) {
    for (int i = 1; i < argc; ++i) {
        if (std::string(*std::next(argv, i)) == "--help") {
            std::cout << "USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq"
                      << std::endl;
            return true;
        }
    }
    return false;
}
}  // namespace zappy::parser
