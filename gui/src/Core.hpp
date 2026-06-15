/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Core
*/

#pragma once

#include <memory>
#include <span>

#include "EventDispatcher.hpp"
#include "Render.hpp"
#include "network/Client.hpp"
#include "strategy/GUIStrategy.hpp"

namespace zappy::gui {
class Core {
  public:
    explicit Core(std::span<char*> args);
    ~Core() = default;
    Core(const Core& other) = delete;
    Core& operator=(const Core& other) = delete;
    Core(Core&& other) = delete;
    Core& operator=(Core&& other) = delete;

    int run();

  private:
    void setup();
    void loop() const;

    std::span<char*> _args;
    parser::GuiConfig _config;
    std::shared_ptr<events::EventDispatcher> _dispatcher;
    std::unique_ptr<graphics::Render> _render;
    std::unique_ptr<network::Client> _client;
};
}  // namespace zappy::gui
