/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Incantation
*/

#include "Incantation.hpp"

#include "game/Player.hpp"
#include "game/World.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace zappy::server::command {

Incantation::Incantation() : ACommand(kTimeLimit) {}

bool start(game::World& world, game::Player& player) {
    auto vecPlayerIds = world.playersWithSameLevelOnTile(player.position(), player.level());

    if (player.checkCondition(world.resourcesAt(player.position()), vecPlayerIds.size())) {
        world.addGuiEvent(shared::protocol::Emitter::build(
            shared::protocol::server::Pic{static_cast<int>(player.position().x), static_cast<int>(player.position().y),
                                          (player.level()), vecPlayerIds}));
        return true;
    }
    return false;
}

void Incantation::execute(game::World& world, game::Player& player) {
    auto vecPlayerIds = world.playersWithSameLevelOnTile(player.position(), player.level());
    bool isSuccess;

    if (player.checkCondition(world.resourcesAt(player.position()), vecPlayerIds.size())) {
        player.addResponse("ok\n");
        isSuccess = true;
    } else {
        player.addResponse("ko\n");
        isSuccess = false;
    }
    world.addGuiEvent(shared::protocol::Emitter::build(shared::protocol::server::Pie{
        static_cast<int>(player.position().x), static_cast<int>(player.position().y), isSuccess}));
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
