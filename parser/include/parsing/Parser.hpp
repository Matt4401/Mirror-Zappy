/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Parser
*/

#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <utility>

#include "exception/ParsingError.hpp"
#include "parsing/IParseStrategy.hpp"

namespace zappy::parser::parsing {
static constexpr std::size_t kExitSuccess = 0;
static constexpr std::size_t kExitFailure = 84;
static constexpr std::string kUsageThrowMessage = "Help displayed";

template <typename ConfigType>
class Parser {
  public:
    explicit Parser(std::unique_ptr<IParseStrategy<ConfigType>> strategy) : _strategy(std::move(strategy)) {}
    ~Parser() = default;

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;
    Parser(Parser&&) = delete;
    Parser& operator=(Parser&&) = delete;

    ConfigType parse(int argc, char** argv) {
        if (!_strategy) {
            throw shared::exception::ParsingError("No parsing strategy provided");
        }

        ConfigType config;
        _strategy->parse(argc, argv, config);
        return config;
    }

  private:
    std::unique_ptr<IParseStrategy<ConfigType>> _strategy;
};
}  // namespace zappy::parser::parsing
