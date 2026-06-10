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
struct GUIConfig;
}

namespace zappy::shared::parsing {
class GUIStrategy : public IParseStrategy<gui::util::GUIConfig> {
  public:
    void parse(int argc, char** argv, gui::util::GUIConfig& config) override;

  private:
    static constexpr std::string kGUIConfigFlags = "p:h:";

    static void validate(const gui::util::GUIConfig& config);
    static bool handleUsage(char** argv, int argc);
    static void parsePort(const std::string& arg, gui::util::GUIConfig& config) ;
    static void processOptions(int argc, char** argv, gui::util::GUIConfig& config);
};
}  // namespace zappy::shared::parsing
