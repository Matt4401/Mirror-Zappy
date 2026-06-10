/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GUIStrategy
*/

#include "parsing/strategy/GUIStrategy.hpp"

#include <iostream>
#include <ostream>
#include <parsing/Parser.hpp>

#include "../../gui/src/util/DataStructures.hpp"
#include "encapsulation/GetoptWrapper.hpp"
#include "exception/ParsingError.hpp"

namespace zappy::shared::parsing {
void GUIStrategy::parse(const int argc, char** argv, gui::util::Config& config) {
    if (handleUsage(argv, argc)) {
        throw exception::ParsingError(kUsageThrowMessage);
    }
    processOptions(argc, argv, config);
    validate(config);
}

void GUIStrategy::processOptions(const int argc, char** argv, gui::util::Config& config) {
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
                throw exception::ParsingError("Invalid GUI option.");
        }
    }
}

void GUIStrategy::parsePort(const std::string& arg, gui::util::Config& config) {
    try {
        const int val = std::stoi(arg);

        if (val <= 0) {
            throw exception::ParsingError("Port must be a positive non-zero integer.");
        }
        config.port = val;
    } catch (const std::exception&) {
        throw exception::ParsingError("Invalid port formatting: '" + arg + "' is not a valid number.");
    }
}

void GUIStrategy::validate(const gui::util::Config& config) {
    if (config.port <= 0) {
        throw exception::ParsingError("Missing or invalid port (-p)");
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
}  // namespace zappy::shared::parsing
