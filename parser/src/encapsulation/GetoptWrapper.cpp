/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GetoptWrapper
*/

#include "encapsulation/GetoptWrapper.hpp"

// NOLINTNEXTLINE
#include <unistd.h>

#include <iterator>
#include <string>
#include <utility>
#include <vector>

namespace zappy::parser::encapsulation {
GetOptWrapper::GetOptWrapper(const int argc, char** argv, std::string optString)
    : _argc(argc), _argv(argv), _optString(std::move(optString)) {
    // NOLINTNEXTLINE
    optind = 1;
    // NOLINTNEXTLINE
    opterr = 0;
    for (int i = 0; i < argc; ++i) {
        _stringArgv.emplace_back(*std::next(argv, i));
    }
}

int GetOptWrapper::getNextOption() const { return getopt(_argc, _argv, _optString.c_str()); }

std::string GetOptWrapper::getOptionArg() {
    // NOLINTNEXTLINE
    if (optarg == nullptr) {
        return "";
    }
    // NOLINTNEXTLINE
    return static_cast<std::string>(optarg);
}

std::vector<std::string> GetOptWrapper::getMultiArgs() const {
    std::vector<std::string> args;

    // NOLINTNEXTLINE
    if (optarg != nullptr) {
        // NOLINTNEXTLINE
        args.emplace_back(optarg);
    }
    // NOLINTNEXTLINE
    while (optind < _argc && _stringArgv.at(optind).at(0) != '-') {
        // NOLINTNEXTLINE
        args.emplace_back(_stringArgv.at(optind));
        // NOLINTNEXTLINE
        optind++;
    }
    return args;
}

// NOLINTNEXTLINE
char GetOptWrapper::getUnknownOption() { return static_cast<char>(optopt); }
}  // namespace zappy::parser::encapsulation
