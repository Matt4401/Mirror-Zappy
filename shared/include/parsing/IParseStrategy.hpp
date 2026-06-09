/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** IParser
*/

#pragma once

namespace zappy::shared::parsing {

template <typename ConfigType>
class IParseStrategy {
  public:
    virtual ~IParseStrategy() = default;

    IParseStrategy() = default;
    IParseStrategy(const IParseStrategy&) = delete;
    IParseStrategy& operator=(const IParseStrategy&) = delete;
    IParseStrategy(IParseStrategy&&) = delete;
    IParseStrategy& operator=(IParseStrategy&&) = delete;

    virtual void parse(int ac, char** av, ConfigType& config) = 0;
};
}  // namespace zappy::shared::parsing
