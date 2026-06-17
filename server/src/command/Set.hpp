/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Set
*/

#pragma once
#include <map>
#include <string>

#include "ACommand.hpp"
#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

const std::map<std::string, game::ItemType> kMapItemString = {
    {"linemate", game::ItemType::Linemate}, {"deraumere", game::ItemType::Deraumere},
    {"sibur", game::ItemType::Sibur},       {"mendiane", game::ItemType::Mendiane},
    {"phiras", game::ItemType::Phiras},     {"thystame", game::ItemType::Thystame},
    {"food", game::ItemType::Food},
};

class Set : public ACommand {
  public:
    Set(std::string arg);
    ~Set() override = default;
    Set(const Set& other) = delete;
    Set& operator=(const Set& other) = delete;
    Set(Set&& other) = delete;
    Set& operator=(Set&& other) = delete;

    bool start(game::World& world, game::Player& player) override;
    void execute(game::World& world, game::Player& player) override;

  private:
    std::string _arg;
    static constexpr int kTimeLimit = 7;
};
}  // namespace zappy::server::command
