/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** SkinGenerator
*/

#pragma once

#include <cstddef>
#include <random>
#include <string>
#include <vector>

namespace zappy::gui::game {

class SkinGenerator {
  public:
    static SkinGenerator& getInstance();

    SkinGenerator(const SkinGenerator&) = delete;
    SkinGenerator& operator=(const SkinGenerator&) = delete;
    SkinGenerator(SkinGenerator&&) = delete;
    SkinGenerator& operator=(SkinGenerator&&) = delete;

    [[nodiscard]] std::string getRandomSkin();

  private:
    SkinGenerator();
    ~SkinGenerator() = default;

    void loadSkinsFromFile(const std::string& path);

    std::vector<std::string> _skins;
    std::discrete_distribution<std::size_t> _distribution;
};

}  // namespace zappy::gui::game
