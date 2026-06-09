/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Core
*/

#pragma once
#include <span>

#include "cli/Parser.hpp"

namespace zappy::gui {
class Core {
  public:
    Core(std::span<char*> args) : _cliParser(cli::Parser(args)) {}
    ~Core() = default;
    Core(const Core& other) = delete;
    Core& operator=(const Core& other) = delete;
    Core(Core&& other) = delete;
    Core& operator=(Core&& other) = delete;

    int run();

  protected:
  private:
    cli::Parser _cliParser;
};
}  // namespace zappy::gui
