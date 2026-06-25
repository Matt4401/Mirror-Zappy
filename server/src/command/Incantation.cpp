/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Incantation
*/

#include "Incantation.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::command {

Incantation::Incantation() : ACommand(kTimeLimit) {}

bool Incantation::playersWithSameLevelOnTileWithMoreFood(const game::Position position, const int level,
                                                         const game::World& world, const std::size_t ogId) {
    const auto [nbPlayer, resources] = game::getCondition().at(level - 1);
    if (nbPlayer <= 1) {
        return true;
    }

    const auto tile = world.tile(position);
    const auto& playerList = world.playerList();
    std::vector<std::size_t> candidates;
    for (const auto& playerId : tile.players) {
        if (playerId == ogId) {
            continue;
        }

        const auto it = playerList.find(playerId);
        if (it != playerList.end() && it->second->level() == level) {
            candidates.push_back(playerId);
        }
    }
    if (candidates.size() + 1 < nbPlayer) {
        return false;
    }

    std::ranges::sort(candidates, [&playerList](const std::size_t a, const std::size_t b) {
        const auto foodA = playerList.at(a)->inventory().at(static_cast<std::uint8_t>(game::ItemType::Food));
        const auto foodB = playerList.at(b)->inventory().at(static_cast<std::uint8_t>(game::ItemType::Food));
        return foodA > foodB;
    });

    for (std::size_t i = 0; i < nbPlayer - 1; ++i) {
        _vecPlayerIds.push_back(candidates.at(i));
    }

    return true;
}

bool Incantation::start(game::World& world, game::Player& player) {
    _vecPlayerIds.clear();
    _vecPlayerIds.push_back(player.id());

    if (!playersWithSameLevelOnTileWithMoreFood(player.position(), player.level(), world, player.id())) {
        _vecPlayerIds.clear();
        return false;
    }

    if (player.checkIncantationRequirements(world.resourcesAt(player.position()))) {
        world.addGuiEvent(shared::protocol::Emitter::build(shared::protocol::server::Pic{
            .x = static_cast<int>(player.position().x),
            .y = static_cast<int>(player.position().y),
            .level = player.level(),
            .playerIds = _vecPlayerIds,
        }));
        for (auto playerId : _vecPlayerIds) {
            const auto& tmpPlayer = world.playerList().at(playerId);
            tmpPlayer->addResponse("Elevation underway\n");
            if (tmpPlayer->id() != player.id()) {
                tmpPlayer->setIncanting(true);
            }
        }
        return true;
    }

    _vecPlayerIds.clear();
    return false;
}

bool Incantation::checkIfPlayersNotDead(const game::World& world) const {
    const auto& player = world.playerList();

    // NOLINTNEXTLINE
    for (auto playerId : _vecPlayerIds) {
        if (!player.contains(playerId)) {
            return false;
        }
    }
    return true;
}

void Incantation::execute(game::World& world, game::Player& player) {
    bool isSuccess = false;

    if (player.checkIncantationRequirements(world.resourcesAt(player.position())) && checkIfPlayersNotDead(world)) {
        auto [nbPlayer, resources] = game::getCondition().at(player.level() - 1);
        for (int i = 0; i < static_cast<int>(game::ItemType::COUNT); ++i) {
            const auto item = static_cast<game::ItemType>(i);
            const std::size_t countNeeded = resources.at(i);

            if (countNeeded > 0) {
                world.removeItemOnGround(item, player.position(), countNeeded);
            }
        }
        for (auto playerId : _vecPlayerIds) {
            const auto& tmpPlayer = world.playerList().at(playerId);
            tmpPlayer->levelUp();
            tmpPlayer->addResponse("Current level: " + std::to_string(tmpPlayer->level()) + "\n");
            tmpPlayer->setIncanting(false);
        }
        isSuccess = true;
        world.addGuiEvent(shared::protocol::Emitter::build(shared::protocol::server::Plv{
            .playerId = static_cast<int>(player.id()),
            .level = player.level(),
        }));
    } else {
        for (auto playerId : _vecPlayerIds) {
            auto it = world.playerList().find(playerId);
            if (it == world.playerList().end()) {
                continue;
            }
            it->second->addResponse("ko\n");
            it->second->setIncanting(false);
        }
        isSuccess = false;
    }
    world.addGuiEvent(shared::protocol::Emitter::build(shared::protocol::server::Pie{
        .x = static_cast<int>(player.position().x),
        .y = static_cast<int>(player.position().y),
        .incantationResult = isSuccess,
    }));
}
}  // namespace zappy::server::command
