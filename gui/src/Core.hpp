/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Core
*/

#pragma once
#include <memory>
#include <span>

#include "Render.hpp"
#include "cli/Parser.hpp"
#include "network/Client.hpp"

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
    std::unique_ptr<graphics::Render> _render;
    std::unique_ptr<network::Client> _client;
};
}  // namespace zappy::gui
