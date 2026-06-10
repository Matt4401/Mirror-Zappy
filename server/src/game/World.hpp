/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World logic and storage
*/

#pragma once

#include "util/DataStructures.hpp"

namespace zappy::server::game {

// TODO: implement world logic and storage once the player, team and world classes are defined
class World {
  public:
    explicit World(const util::Config& config);
    ~World() = default;

    World(const World& other) = delete;
    World& operator=(const World& other) = delete;
    World(World&& other) = delete;
    World& operator=(World&& other) = delete;

    static std::pair<std::size_t, std::size_t> getLimitMap() {
        return {16, 16};  // FIXME: put reel map limit get by the parser.
    }

  private:
};
}  // namespace zappy::server::game
