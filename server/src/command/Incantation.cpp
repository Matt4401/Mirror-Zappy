/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Incantation
*/

#include "Incantation.hpp"

#include <array>
#include <cstddef>

#include "ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::command {

Incantation::Incantation() : ACommand(kTimeLimit) {}

bool Incantation::start(game::World& world, game::Player& player) {
    auto vecPlayerIds = world.playersWithSameLevelOnTile(player.position(), player.level());

    if (player.checkCondition(world.resourcesAt(player.position()), vecPlayerIds.size())) {
        world.addGuiEvent(
            shared::protocol::Emitter::build(shared::protocol::server::Pic{.x = static_cast<int>(player.position().x),
                                                                           .y = static_cast<int>(player.position().y),
                                                                           .level = player.level(),
                                                                           .playerIds = vecPlayerIds}));
        return true;
    }
    return false;
}

void Incantation::execute(game::World& world, game::Player& player) {
    const auto vecPlayerIds = world.playersWithSameLevelOnTile(player.position(), player.level());
    bool isSuccess = false;

    if (player.checkCondition(world.resourcesAt(player.position()), vecPlayerIds.size())) {
        player.addResponse("ok\n");
        auto [nbPlayer, resources] = getCondition().at(player.level() - 1);
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
    world.addGuiEvent(
        shared::protocol::Emitter::build(shared::protocol::server::Pie{.x = static_cast<int>(player.position().x),
                                                                       .y = static_cast<int>(player.position().y),
                                                                       .incantationResult = isSuccess}));
}

std::array<game::Condition, game::kNbLevel> getCondition() {
    static constexpr std::array kCondition = {game::Condition{.nbPlayer = 1, .resources = {0, 1, 0, 0, 0, 0, 0}},
                                              game::Condition{.nbPlayer = 2, .resources = {0, 1, 1, 1, 0, 0, 0}},
                                              game::Condition{.nbPlayer = 2, .resources = {0, 2, 0, 1, 0, 2, 0}},
                                              game::Condition{.nbPlayer = 4, .resources = {0, 1, 1, 2, 0, 1, 0}},
                                              game::Condition{.nbPlayer = 4, .resources = {0, 1, 2, 1, 3, 0, 0}},
                                              game::Condition{.nbPlayer = 6, .resources = {0, 1, 2, 3, 0, 1, 0}},
                                              game::Condition{.nbPlayer = 6, .resources = {0, 2, 2, 2, 2, 2, 1}}};
    return kCondition;
}
}  // namespace zappy::server::command
