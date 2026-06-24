/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Incantation
*/

#include "Incantation.hpp"

#include <array>
#include <cstddef>
#include <vector>

#include "ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::command {

Incantation::Incantation() : ACommand(kTimeLimit) {}

std::vector<std::size_t> Incantation::playersWithSameLevelOnTile(const game::Position position, const int level,
                                                                 const game::World& world) {
    const auto tile = world.tile(position);
    const auto& playerList = world.playerList();
    const auto playerIds = tile.players;
    std::vector<std::size_t> vecPlayerId{};
    for (const auto& playerId : playerIds) {
        if (playerList.find(playerId)->second->level() == level) {
            vecPlayerId.push_back(playerId);
        }
    }
    return vecPlayerId;
}

bool Incantation::start(game::World& world, game::Player& player) {
    auto vecPlayerIds = playersWithSameLevelOnTile(player.position(), player.level(), world);

    if (player.checkIncantationRequirements(world.resourcesAt(player.position()), vecPlayerIds.size())) {
        world.addGuiEvent(shared::protocol::Emitter::build(shared::protocol::server::Pic{
            .x = static_cast<int>(player.position().x),
            .y = static_cast<int>(player.position().y),
            .level = player.level(),
            .playerIds = vecPlayerIds,
        }));
        return true;
    }
    return false;
}

void Incantation::execute(game::World& world, game::Player& player) {
    const auto vecPlayerIds = playersWithSameLevelOnTile(player.position(), player.level(), world);
    bool isSuccess = false;

    if (player.checkIncantationRequirements(world.resourcesAt(player.position()), vecPlayerIds.size())) {
        player.addResponse("ok\n");
        auto [nbPlayer, resources] = game::getCondition().at(player.level() - 1);
        for (int i = 0; i < static_cast<int>(game::ItemType::COUNT); ++i) {
            const auto item = static_cast<game::ItemType>(i);
            const std::size_t countNeeded = resources.at(i);

            if (countNeeded > 0) {
                world.removeItemOnGround(item, player.position(), countNeeded);
            }
        }
        player.levelUp();
        isSuccess = true;
    } else {
        player.addResponse("ko\n");
        isSuccess = false;
    }
    world.addGuiEvent(shared::protocol::Emitter::build(shared::protocol::server::Pie{
        .x = static_cast<int>(player.position().x),
        .y = static_cast<int>(player.position().y),
        .incantationResult = isSuccess,
    }));
}
}  // namespace zappy::server::command
