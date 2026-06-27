/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** NameGenerator
*/

#pragma once

#include <string>
#include <vector>

namespace zappy::gui::game {

class NameGenerator {
  public:
    static NameGenerator& getInstance();

    NameGenerator(const NameGenerator&) = delete;
    NameGenerator& operator=(const NameGenerator&) = delete;
    NameGenerator(NameGenerator&&) = delete;
    NameGenerator& operator=(NameGenerator&&) = delete;

    [[nodiscard]] std::string getRandomName();

  private:
    NameGenerator();
    ~NameGenerator() = default;

    void loadNamesFromFile(const std::string& path);

    std::vector<std::string> _names;
};

}  // namespace zappy::gui::game
