/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Player
*/

#pragma once

namespace zappy::server::game {
class Player {
  public:
    Player(const Player& other) = delete;
    Player& operator=(const Player& other) = delete;
    Player(Player&& other) = delete;
    Player& operator=(Player&& other) = delete;
};
}  // namespace zappy::server::game
