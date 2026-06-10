/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GUIStrategy
*/

#pragma once

#include <string>

#include "parsing/IParseStrategy.hpp"

namespace zappy::gui::util {
struct Config;
}

namespace zappy::shared::parsing {
class GUIStrategy : public IParseStrategy<gui::util::Config> {
  public:
    void parse(int argc, char** argv, gui::util::Config& config) override;

  private:
    static constexpr std::string kGUIConfigFlags = "p:h:";

    static void validate(const gui::util::Config& config);
    static bool handleUsage(char** argv, int argc);
    static void parsePort(const std::string& arg, gui::util::Config& config) ;
    static void processOptions(int argc, char** argv, gui::util::Config& config);
};
}  // namespace zappy::shared::parsing
