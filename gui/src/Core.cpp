/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Core
*/

#include "Core.hpp"

#include <iostream>
#include <memory>

#include "graphics/Render.hpp"
#include "network/Client.hpp"
#include "parsing/Parser.hpp"

namespace zappy::gui {
Core::Core(const std::span<char*> args) : _args(args) {}

int Core::run() {
    try {
        setup();
        loop();
    } catch (const zappy::shared::exception::Exception& e) {
        if (std::string(e.what()) == zappy::parser::parsing::kUsageThrowMessage) {
            return zappy::parser::parsing::kExitSuccess;
        }
        std::cerr << "Error: " << e.what() << std::endl;
        return zappy::parser::parsing::kExitFailure;
    } catch (const std::exception& e) {
        std::cerr << "Unknown error: " << e.what() << std::endl;
        return zappy::parser::parsing::kExitFailure;
    }
    return zappy::parser::parsing::kExitSuccess;
}

void Core::setup() {
    auto guiStrategy = std::make_unique<zappy::parser::parsing::GUIStrategy>();
    zappy::parser::parsing::Parser<zappy::parser::parsing::GuiConfig> parser(std::move(guiStrategy));

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
