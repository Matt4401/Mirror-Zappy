/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** SkinGenerator
*/

#include "SkinGenerator.hpp"

#include <cstddef>
#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace zappy::gui::game {

SkinGenerator& SkinGenerator::getInstance() {
    static SkinGenerator instance;
    return instance;
}

SkinGenerator::SkinGenerator() { loadSkinsFromFile("assets/config/skins.txt"); }

void SkinGenerator::loadSkinsFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return;
    }

    std::vector<double> weights;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string skinPath;
        double weight = 1.0;

        if (iss >> skinPath) {
            if (!(iss >> weight)) {
                weight = 10.0;
            }
            _skins.push_back(skinPath);
            weights.push_back(weight);
        }
    }

    if (!weights.empty()) {
        _distribution = std::discrete_distribution<std::size_t>(weights.begin(), weights.end());
    }
}

std::string SkinGenerator::getRandomSkin() {
    if (_skins.empty()) {
        return "";
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return _skins[_distribution(gen)];
}

}  // namespace zappy::gui::game
