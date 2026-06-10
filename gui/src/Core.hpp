/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Core
*/

#pragma once

#include <memory>
#include <utility>

#include "Render.hpp"
#include "network/Client.hpp"
#include "util/DataStructures.hpp"

namespace zappy::gui {
class Core {
  public:
    explicit Core(util::Config config) : _config(std::move(config)) {};
    ~Core() = default;
    Core(const Core& other) = delete;
    Core& operator=(const Core& other) = delete;
    Core(Core&& other) = delete;
    Core& operator=(Core&& other) = delete;

    void run();

  private:
    util::Config _config;
    std::unique_ptr<graphics::Render> _render;
    std::unique_ptr<network::Client> _client;
};
}  // namespace zappy::gui
