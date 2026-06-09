/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GUIStrategy
*/

#include "parsing/strategy/GUIStrategy.hpp"

#include <iostream>
#include <ostream>

#include "../../gui/src/util/DataStructures.hpp"
#include "encapsulation/GetoptWrapper.hpp"
#include "exception/ParsingError.hpp"

namespace zappy::shared::parsing {
void GUIStrategy::parse(const int argc, char** argv, gui::util::GUIConfig& config) {
    const encapsulation::GetOptWrapper optionParser(argc, argv, kGUIConfigFlags);
    int opt = 0;

    while ((opt = optionParser.getNextOption()) != -1) {
        switch (opt) {
            case 'p':
                config.port = std::stoi(encapsulation::GetOptWrapper::getOptionArg());
                break;
            case 'h':
                config.machine = encapsulation::GetOptWrapper::getOptionArg();
                break;
            default:
                throw exception::ParsingError("Invalid GUI option.");
        }
    }
}

void GUIStrategy::validate(const gui::util::GUIConfig& config) {
    if (config.port <= 0) {
        throw exception::ParsingError("Missing or invalid port (-p)");
    }
}

bool GUIStrategy::printUsage(char** argv, const int argc) {
    for (int i = 1; i < argc; ++i) {
        if (std::string(*std::next(argv, i)) == "--help") {
            std::cout << "USAGE: ./zappy_gui -p port -h machine" << std::endl;
            return true;
        }
    }
    return false;
}
}  // namespace zappy::shared::parsing
