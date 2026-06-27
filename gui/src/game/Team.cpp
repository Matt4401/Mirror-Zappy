/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Team
*/

#include "Team.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "Color.hpp"
#include "Egg.hpp"
#include "GameModel.hpp"
#include "Player.hpp"
#include "game/NameGenerator.hpp"
#include "graphics/AssetManager.hpp"
#include "rmath/Vector3.hpp"
#include "rtextures/Texture2D.hpp"

namespace zappy::gui::game {
void Team::draw(const GameModel& gameModel) const {
    for (const auto& player : _players) {
        std::shared_ptr<raylib::rtextures::Texture2D> tex = nullptr;
        if (!player.textureId().empty()) {
            tex = graphics::AssetManager::getInstance().getTexture(player.textureId());
        }
        gameModel.drawPlayer(player.position(), player.renderRotationAngle(), player.action(), player.animFrame(), tex,
                             player.level());
    }
    for (const auto& egg : _eggs) {
        const auto tint = raylib::Color::lerp(raylib::Color::White(), _teamColor, GameModel::EGG_TINT_STRENGTH);
        gameModel.drawEgg(egg.position(), tint);
    }
}

Player& Team::addPlayer(int id, raylib::rmath::Vector3 position, Player::cardinalPoint orientation, std::size_t level) {
    if (const auto existing = findPlayer(id); existing.has_value()) {
        return existing->get();
    }
    _players.emplace_back(id, position, NameGenerator::getInstance().getRandomName(), orientation, level);
    return _players.back();
}

void Team::updatePlayerName(int id, const std::string& name) {
    auto player = std::ranges::find_if(_players, [id](const Player& player) { return player.id() == id; });

    if (player != _players.end()) {
        player->setName(name);
    }
}

bool Team::hasPlayer(int id) const {
    return std::ranges::any_of(_players, [id](const Player& player) { return player.id() == id; });
}

std::optional<std::reference_wrapper<Player>> Team::findPlayer(int id) {
    const auto player = std::ranges::find_if(_players, [id](const Player& candidate) { return candidate.id() == id; });
    if (player == _players.end()) {
        return std::nullopt;
    }
    return std::ref(*player);
}

std::optional<std::reference_wrapper<const Player>> Team::findPlayer(int id) const {
    const auto player = std::ranges::find_if(_players, [id](const Player& candidate) { return candidate.id() == id; });
    if (player == _players.end()) {
        return std::nullopt;
    }
    return std::cref(*player);
}

void Team::removePlayer(int id) {
    std::erase_if(_players, [id](const Player& player) { return player.id() == id; });
}

void Team::addEgg(int id, int playerId, const raylib::rmath::Vector3& position) {
    _eggs.emplace_back(id, playerId, position);
}

void Team::removeEgg(int id) {
    std::erase_if(_eggs, [id](const Egg& egg) { return egg.id() == id; });
}

void Team::movePlayers(const int serverFrequency, const float deltaTime) {
    for (auto& player : _players) {
        player.move(serverFrequency, deltaTime);
    }
}
}  // namespace zappy::gui::game
