/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World logic and storage
*/

#pragma once

#include <cstddef>

#include "parsing/strategy/ServerStrategy.hpp"

namespace zappy::server::game {

struct pos {
    std::size_t x;
    std::size_t y;
};

// TODO: implement world logic and storage once the player, team and world classes are defined
class World {
  public:
    explicit World(const zappy::parser::parsing::ServerConfig& config);
    ~World() = default;

    World(const World& other) = delete;
    World& operator=(const World& other) = delete;
    World(World&& other) = delete;
    World& operator=(World&& other) = delete;

    static pos limitMap() {         // TODO : don't forget to remove the static when u have
                                    // real limit and also change so test that use that (an issue is open for that)
        return {.x = 16, .y = 16};  // FIXME: put real map limit get by the parser.
    }

  private:
};
}  // namespace zappy::server::game
