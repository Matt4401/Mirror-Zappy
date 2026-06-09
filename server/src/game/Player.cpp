/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Player
*/

#include "Player.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "command/ICommand.hpp"
#include "exception/ToMuchCmd.hpp"
#include "game/World.hpp"

namespace zappy::server::game {
Player::Player(const int id, std::size_t x, std::size_t y) : _orientation(orientation::NORTH), _pos({x, y}), _id(id) {
    _inventory.fill(0);
    setItem(ItemType::Food, 10);
}

void Player::addItem(ItemType item, const std::size_t quantity) {
    _inventory.at(static_cast<uint8_t>(item)) += quantity;
    if (item == ItemType::Food) {
        _lifeTick += 126 * quantity;
    }
}

void Player::subItem(ItemType item, const std::size_t quantity) {
    _inventory.at(static_cast<uint8_t>(item)) -= quantity;
}

std::size_t Player::getItem(ItemType item) const { return _inventory.at(static_cast<uint8_t>(item)); }

void Player::setItem(ItemType item, const size_t amount) { _inventory.at(static_cast<size_t>(item)) = amount; }

void Player::pushCommand(std::unique_ptr<command::ICommand> command) {
    if (_commands.size() >= 10) {
        throw exception::ToMuchCmd{"Player " + std::to_string(_id) + " has too much commands queued"};
    }
    if (_currentCommand == nullptr) {
        _currentCommand = std::move(command);
        _cmdTick = _currentCommand->requiredTicks();
        return;
    }
    _commands.push(std::move(command));
}

void Player::update(World& world) {
    _lifeTick--;
    if (_cmdTick == 0) {
        if (_commands.empty()) {
            return;
        }
        _currentCommand = std::move(_commands.front());
        _commands.pop();
        _currentCommand->start(world, *this);
        return;
    }
    _cmdTick--;
}

void Player::moveUp(const std::pair<std::size_t, std::size_t>& limit) {
    auto [fst, snd] = playerMove.at(static_cast<uint8_t>(_orientation));
    const std::size_t width = limit.first + 1;
    const std::size_t height = limit.second + 1;

    _pos.first = (_pos.first + fst + width) % width;
    _pos.second = (_pos.second + snd + height) % height;
}

void Player::addResponse(const std::string& str) { _buffersResponses.push_back(str); }

std::vector<std::string> Player::getResponses() {
    auto tmpResponses = _buffersResponses;
    _buffersResponses.clear();
    return tmpResponses;
}

std::pair<std::size_t, std::size_t> Player::getPosition() { return _pos; }

}  // namespace zappy::server::game
