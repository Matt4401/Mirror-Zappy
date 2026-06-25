/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Incantation
*/

#pragma once

#include <cstddef>
#include <vector>

#include "ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

class Incantation : public ACommand {
  public:
    Incantation();
    ~Incantation() override = default;
    Incantation(const Incantation& other) = delete;
    Incantation& operator=(const Incantation& other) = delete;
    Incantation(Incantation&& other) = delete;
    Incantation& operator=(Incantation&& other) = delete;

    bool start(game::World& world, game::Player& player) override;
    [[nodiscard]] bool checkIfPlayersNotDead(const game::World& world) const;
    void execute(game::World& world, game::Player& player) override;

  private:
    std::vector<std::size_t> _vecPlayerIds;

    static constexpr int kTimeLimit = 300;
    bool playersWithSameLevelOnTileWithMoreFood(game::Position position, int level, const game::World& world,
                                                size_t ogId);
};
}  // namespace zappy::server::command
