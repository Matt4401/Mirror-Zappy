/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Player
*/

#include "game/Player.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "command/ICommand.hpp"
#include "exception/TooMuchCmd.hpp"
#include "game/World.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"

namespace {
constexpr bool hasEnoughResources(const zappy::server::game::InventoryArray& groundInv,
                                  const zappy::server::game::InventoryArray& required) {
    for (std::size_t i = 0; i < groundInv.size(); ++i) {
        if (groundInv.at(i) < required.at(i)) {
            return false;
        }
    }
    return true;
}
}  // namespace

namespace zappy::server::game {

Player::Player(const std::size_t id, const std::size_t x, const std::size_t y, const cardinalPoint orient)
    : _orientation(orient), _lifeTick(kNbLifeTickFood), _pos({.x = x, .y = y}), _id(id) {
    _inventory.fill(0);
    setItem(ItemType::Food, kNbStartFood);
}

void Player::addItem(ItemType item, const std::size_t quantity) {
    _inventory.at(static_cast<std::uint8_t>(item)) += quantity;
}

bool Player::subItem(ItemType item, const std::size_t quantity) {
    auto& nbInventory = _inventory.at(static_cast<std::uint8_t>(item));
    if (nbInventory < quantity) {
        return false;
    }
    nbInventory -= quantity;
    return true;
}

std::size_t Player::getItem(ItemType item) const { return _inventory.at(static_cast<std::uint8_t>(item)); }

void Player::setItem(ItemType item, const size_t amount) { _inventory.at(static_cast<std::size_t>(item)) = amount; }

void Player::pushCommand(std::unique_ptr<command::ICommand> command) {
    if (_commands.size() + (_currentCommand != nullptr ? 1 : 0) >= kMaxNbCmd) {
        throw exception::TooMuchCmd{"Player " + std::to_string(_id) + " has too much commands queued"};
    }
    _commands.push(std::move(command));
}

void Player::update(World& world) {
    _lifeTick--;
    if (_lifeTick <= 0) {
        if (_inventory.at(static_cast<std::uint8_t>(ItemType::Food)) > 0) {
            _lifeTick = kNbLifeTickFood;
            _inventory.at(static_cast<std::uint8_t>(ItemType::Food))--;
            world.addGuiEvent(shared::protocol::Emitter::build(shared::protocol::server::Pin{
                .playerId = static_cast<int>(_id),
                .x = static_cast<int>(_pos.x),
                .y = static_cast<int>(_pos.y),
                .food = static_cast<int>(_inventory.at(static_cast<std::uint8_t>(ItemType::Food))),
                .linemate = static_cast<int>(_inventory.at(static_cast<std::uint8_t>(ItemType::Linemate))),
                .deraumere = static_cast<int>(_inventory.at(static_cast<std::uint8_t>(ItemType::Deraumere))),
                .sibur = static_cast<int>(_inventory.at(static_cast<std::uint8_t>(ItemType::Sibur))),
                .mendiane = static_cast<int>(_inventory.at(static_cast<std::uint8_t>(ItemType::Mendiane))),
                .thystame = static_cast<int>(_inventory.at(static_cast<std::uint8_t>(ItemType::Thystame))),
            }));

        } else {
            world.removePlayer(_id);
            return;
        }
    }
    if (_isNewCommand) {
        _isNewCommand = false;
    } else if (_cmdTick > 0) {
        _cmdTick--;
    }
    if (_cmdTick == 0 && _currentCommand != nullptr) {
        _currentCommand->execute(world, *this);
        _currentCommand = nullptr;
    }
    if (_currentCommand == nullptr) {
        tryStartNextCommand(world, false);
    }
}

void Player::tryStartNextCommand(World& world, bool isMidTick) {
    if (_cmdTick > 0 || _currentCommand != nullptr) {
        return;
    }

    while (!_commands.empty()) {
        _currentCommand = std::move(_commands.front());
        _commands.pop();

        if (!_currentCommand->start(world, *this)) {
            _buffersResponses.emplace_back("ko\n");
            _currentCommand = nullptr;
        } else {
            _cmdTick = _currentCommand->requiredTicks();
            _isNewCommand = isMidTick;
            return;
        }
    }
}

void Player::moveForward(const Position& limit) {
    auto [fst, snd] = playerMove.at(static_cast<std::uint8_t>(_orientation));
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
    auto [fst, snd] = playerMove.at(static_cast<std::uint8_t>(orientation));
    const std::size_t width = limit.x;
    const std::size_t height = limit.y;

    _pos.x = (_pos.x + fst + width) % width;
    _pos.y = (_pos.y + snd + height) % height;
}

int Player::cmdTick() const { return static_cast<int>(_cmdTick); }

bool Player::hasCommands() const { return _currentCommand != nullptr || !_commands.empty(); }

void Player::setPosition(const Position pos) { _pos = pos; }

Position Player::getLeftTile(const Position ogPos, const Position& mapLimit) {
    auto [fst, snd] = leftTile.at(static_cast<uint8_t>(_orientation));
    const std::size_t width = mapLimit.x;
    const std::size_t height = mapLimit.y;
    Position newPos{};

    newPos.x = (ogPos.x + fst + width) % width;
    newPos.y = (ogPos.y + snd + height) % height;

    return newPos;
}

std::array<std::size_t, static_cast<uint8_t>(ItemType::COUNT)> Player::inventory() const { return _inventory; }

std::vector<Position> Player::getLookPos(const Position mapLimit) {
    std::vector pos{{Position{.x = _pos.x, .y = _pos.y}}};
    pos.reserve((static_cast<std::size_t>(_level) + 1) * (static_cast<std::size_t>(_level) + 1));

    for (int i = 0; i < _level; i++) {
        const Position actualDiagPos = getNthDiagonalLeftPosition(i + 1, mapLimit);
        pos.emplace_back(actualDiagPos);
        const int stepsToRight = 2 * (i + 1);
        for (int j = 0; j < stepsToRight; j++) {
            pos.emplace_back(getLeftTile(pos.back(), mapLimit));
        }
    }
    return pos;
}

Position Player::getNthDiagonalLeftPosition(const std::size_t n, const Position mapLimit) {
    auto [baseDx, baseDy] = diagonalLeftMove.at(static_cast<std::uint8_t>(_orientation));

    const int width = static_cast<int>(mapLimit.x);
    const int height = static_cast<int>(mapLimit.y);
    const int step = static_cast<int>(n);

    const int dx = baseDx * step;
    const int dy = baseDy * step;

    int newX = (static_cast<int>(_pos.x) + dx) % width;
    if (newX < 0) {
        newX += width;
    }
    int newY = (static_cast<int>(_pos.y) + dy) % height;
    if (newY < 0) {
        newY += height;
    }

    return Position{.x = static_cast<std::size_t>(newX), .y = static_cast<std::size_t>(newY)};
}

int Player::level() const { return _level; }

void Player::levelUp() {
    if (_level <= kNbLevel) {
        _level++;
    }
}

bool Player::checkIncantationRequirements(
    const std::array<std::size_t, static_cast<uint8_t>(ItemType::COUNT)>& resources, const std::size_t nbPlayer) const {
    auto condition = getCondition().at(_level - 1);
    return condition.nbPlayer <= nbPlayer && hasEnoughResources(resources, condition.resources);
}

}  // namespace zappy::server::game
