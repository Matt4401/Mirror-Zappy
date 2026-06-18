/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Player
*/

#include "game/Player.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "command/ICommand.hpp"
#include "exception/TooMuchCmd.hpp"
#include "game/World.hpp"

namespace zappy::server::game {

Player::Player(const std::size_t id, const std::size_t x, const std::size_t y, const cardinalPoint orient)
    : _orientation(orient), _lifeTick(kNbStartFood * kNbLifeTickFood), _pos({.x = x, .y = y}), _id(id) {
    _inventory.fill(0);
    setItem(ItemType::Food, kNbStartFood);
}

void Player::addItem(ItemType item, const std::size_t quantity) {
    _inventory.at(static_cast<uint8_t>(item)) += quantity;
    if (item == ItemType::Food) {
        _lifeTick += kNbLifeTickFood * quantity;
    }
}

void Player::subItem(ItemType item, const std::size_t quantity) {
    const auto nbInventory = _inventory.at(static_cast<uint8_t>(item));
    if (nbInventory < quantity) {
        return;
    }
    _inventory.at(static_cast<uint8_t>(item)) -= quantity;
}

std::size_t Player::getItem(ItemType item) const { return _inventory.at(static_cast<uint8_t>(item)); }

void Player::setItem(ItemType item, const size_t amount) { _inventory.at(static_cast<size_t>(item)) = amount; }

void Player::pushCommand(std::unique_ptr<command::ICommand> command) {
    if (_commands.size() >= kMaxNbCmd) {
        throw exception::TooMuchCmd{"Player " + std::to_string(_id) + " has too much commands queued"};
    }
    _commands.push(std::move(command));
}

void Player::update(World& world) {
    _lifeTick--;

    if (_cmdTick > 0) {
        _cmdTick--;
    }
    while (_cmdTick == 0) {
        if (_currentCommand != nullptr) {
            _currentCommand->execute(world, *this);
            _currentCommand = nullptr;
        }
        if (_commands.empty()) {
            break;
        }
        _currentCommand = std::move(_commands.front());
        _commands.pop();

        if (!_currentCommand->start(world, *this)) {
            _buffersResponses.emplace_back("ko\n");
            _currentCommand = nullptr;
            continue;
        }
        _cmdTick = _currentCommand->requiredTicks();
        if (_cmdTick > 0) {
            break;
        }
    }
}

void Player::moveForward(const Position& limit) {
    auto [fst, snd] = playerMove.at(static_cast<uint8_t>(_orientation));
    const int width = static_cast<int>(limit.x);
    const int height = static_cast<int>(limit.y);
    const int newX = (static_cast<int>(_pos.x) + fst + width) % width;
    const int newY = (static_cast<int>(_pos.y) + snd + height) % height;

    _pos.x = static_cast<std::size_t>(newX);
    _pos.y = static_cast<std::size_t>(newY);
}

void Player::addResponse(const std::string& str) { _buffersResponses.push_back(str); }

std::vector<std::string> Player::responses() {
    auto tmpResponses = _buffersResponses;
    _buffersResponses.clear();
    return tmpResponses;
}

Position Player::position() const { return _pos; }

void Player::setOrientation(const cardinalPoint orient) { _orientation = orient; }

cardinalPoint Player::orientation() const { return _orientation; }

std::size_t Player::id() const { return _id; }

std::size_t Player::nbLifeTick() const { return _lifeTick; }

void Player::kill() {
    _isDead = true;
    _buffersResponses = {"dead\n"};
}

void Player::moveWithOrientation(const Position& limit, cardinalPoint orientation) {
    auto [fst, snd] = playerMove.at(static_cast<uint8_t>(orientation));
    const std::size_t width = limit.x;
    const std::size_t height = limit.y;

    _pos.x = (_pos.x + fst + width) % width;
    _pos.y = (_pos.y + snd + height) % height;
}

int Player::cmdTick() const { return static_cast<int>(_cmdTick); }

bool Player::hasCommands() const { return _currentCommand != nullptr || !_commands.empty(); }

void Player::setPosition(const Position pos) { _pos = pos; }

}  // namespace zappy::server::game
