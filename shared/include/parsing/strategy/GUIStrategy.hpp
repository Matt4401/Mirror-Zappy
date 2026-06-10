/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GUIStrategy
*/

#pragma once

#include <cstdint>
#include <string>

#include "parsing/IParseStrategy.hpp"

namespace zappy::shared::parsing {
struct GuiConfig {
    std::uint16_t port = 0;
    std::string machine = "127.0.0.1";
};

class GUIStrategy : public IParseStrategy<GuiConfig> {
  public:
    void parse(int argc, char** argv, GuiConfig& config) override;

  private:
    static constexpr std::string kGUIConfigFlags = "p:h:";

    static void validate(const GuiConfig& config);
    static bool handleUsage(char** argv, int argc);
    static void parsePort(const std::string& arg, GuiConfig& config);
    static void processOptions(int argc, char** argv, GuiConfig& config);
};
}  // namespace zappy::shared::parsing
