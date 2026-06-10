/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GUIStrategy
*/

#include "parsing/strategy/GUIStrategy.hpp"

#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <exception>

#include "encapsulation/GetoptWrapper.hpp"
#include "exception/ParsingError.hpp"
#include "parsing/Parser.hpp"

namespace zappy::parser::parsing {
void GUIStrategy::parse(const int argc, char** argv, GuiConfig& config) {
    if (handleUsage(argv, argc)) {
        throw shared::exception::ParsingError(kUsageThrowMessage);
    }
    processOptions(argc, argv, config);
    validate(config);
}

void GUIStrategy::processOptions(const int argc, char** argv, GuiConfig& config) {
    const encapsulation::GetOptWrapper optionParser(argc, argv, kGUIConfigFlags);
    int opt = 0;

    while ((opt = optionParser.getNextOption()) != -1) {
        switch (opt) {
            case 'p':
                parsePort(encapsulation::GetOptWrapper::getOptionArg(), config);
                break;
            case 'h':
                config.machine = encapsulation::GetOptWrapper::getOptionArg();
                break;
            default:
                throw shared::exception::ParsingError("Invalid GUI option.");
        }
    }
}

void GUIStrategy::parsePort(const std::string& arg, GuiConfig& config) {
    try {
        const int val = std::stoi(arg);

        if (val <= 0) {
            throw shared::exception::ParsingError("Port must be a positive non-zero integer.");
        }
        config.port = val;
    } catch (const std::exception&) {
        throw shared::exception::ParsingError("Invalid port formatting: '" + arg + "' is not a valid number.");
    }
}

void GUIStrategy::validate(const GuiConfig& config) {
    if (config.port <= 0) {
        throw shared::exception::ParsingError("Missing or invalid port (-p)");
    }
}

bool GUIStrategy::handleUsage(char** argv, const int argc) {
    for (int i = 1; i < argc; ++i) {
        if (std::string(*std::next(argv, i)) == "--help") {
            std::cout << "USAGE: ./zappy_gui -p port -h machine" << std::endl;
            return true;
        }
    }
    return false;
}
}  // namespace zappy::parser::parsing
