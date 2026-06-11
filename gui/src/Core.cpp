/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Core
*/

#include "Core.hpp"

#include <exception>
#include <iostream>
#include <memory>
#include <span>
#include <string>
#include <utility>

#include "Parser.hpp"
#include "exception/Exception.hpp"
#include "graphics/Render.hpp"
#include "network/Client.hpp"
#include "strategy/GUIStrategy.hpp"

namespace zappy::gui {
Core::Core(const std::span<char*> args) : _args(args) {}

int Core::run() {
    try {
        setup();
        loop();
    } catch (const zappy::shared::exception::Exception& e) {
        if (std::string(e.what()) == zappy::parser::kUsageThrowMessage) {
            return zappy::parser::kExitSuccess;
        }
        std::cerr << "Error: " << e.what() << std::endl;
        return zappy::parser::kExitFailure;
    } catch (const std::exception& e) {
        std::cerr << "Unknown error: " << e.what() << std::endl;
        return zappy::parser::kExitFailure;
    }
    return zappy::parser::kExitSuccess;
}

void Core::setup() {
    auto guiStrategy = std::make_unique<zappy::parser::GUIStrategy>();
    zappy::parser::Parser<zappy::parser::GuiConfig> parser(std::move(guiStrategy));

    _config = parser.parse(static_cast<int>(_args.size()), _args.data());
    _client = std::make_unique<network::Client>(_config);
    _render = std::make_unique<graphics::Render>();
}

void Core::loop() const {
    if (_render) {
        _render->start();
    }
}
}  // namespace zappy::gui
