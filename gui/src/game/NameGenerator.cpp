/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** NameGenerator
*/

#include "NameGenerator.hpp"

#include <cstddef>
#include <fstream>
#include <random>
#include <string>

namespace zappy::gui::game {

NameGenerator& NameGenerator::getInstance() {
    static NameGenerator instance;
    return instance;
}

NameGenerator::NameGenerator() {
    loadNamesFromFile("assets/config/player_names.txt");
    if (_names.empty()) {
        _names = {"Zorg", "Xylar", "Quasar", "Triton"};
    }
}

void NameGenerator::loadNamesFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            _names.push_back(line);
        }
    }
}

std::string NameGenerator::getRandomName() {
    if (_names.empty()) {
        return "Unknown";
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dist(0, _names.size() - 1);
    return _names.at(dist(gen));
}

}  // namespace zappy::gui::game
