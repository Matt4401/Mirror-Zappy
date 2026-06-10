/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World logic and storage
*/

#pragma once

#include "parsing/strategy/ServerStrategy.hpp"
#include "util/DataStructures.hpp"

namespace zappy::server::game {

// TODO: implement world logic and storage once the player, team and world classes are defined
class World {
  public:
    explicit World(const zappy::shared::parsing::ServerConfig& config);
    ~World() = default;

    World(const World& other) = delete;
    World& operator=(const World& other) = delete;
    World(World&& other) = delete;
    World& operator=(World&& other) = delete;

  private:
};
}  // namespace zappy::server::game
