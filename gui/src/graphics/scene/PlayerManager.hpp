/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** PlayerManager
*/

#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "Color.hpp"
#include "TileManager.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "gui/src/game/Player.hpp"
#include "gui/src/game/Team.hpp"
#include "protocol/Commands.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::graphics::scene {
struct Incantation {
    int x{0};
    int y{0};
    int level{0};
    std::vector<std::size_t> playerIds;
};

class PlayerManager {
  public:
    static constexpr auto DefaultPlayerOffsetY = 1.4F;
    static constexpr std::array<raylib::Color, 8> TeamColors = {
        raylib::Color::Red(),    raylib::Color::Blue(),   raylib::Color::Green(),   raylib::Color::Yellow(),
        raylib::Color::Purple(), raylib::Color::Orange(), raylib::Color::SkyBlue(), raylib::Color::Pink(),
    };

    explicit PlayerManager(TileManager& tileManager) : _tileManager(tileManager) {}
    ~PlayerManager() = default;
    PlayerManager(const PlayerManager& other) = delete;
    PlayerManager& operator=(const PlayerManager& other) = delete;
    PlayerManager(PlayerManager&& other) = delete;
    PlayerManager& operator=(PlayerManager&& other) = delete;

    [[nodiscard]] const std::vector<game::Team>& teams() const { return _teams; }
    [[nodiscard]] const std::vector<Incantation>& activeIncantations() const { return _activeIncantations; }
    [[nodiscard]] std::optional<std::reference_wrapper<const game::Player>> playerById(int id) const;
    void updatePlayerName(int id, const std::string& name);

    void handleTeamName(const shared::protocol::server::Tna& command);
    void handlePlayerConnected(const shared::protocol::server::Pnw& command);
    void handlePlayerPosition(const shared::protocol::server::Ppo& command);
    void handlePlayerLevel(const shared::protocol::server::Plv& command);
    void handlePlayerInventory(const shared::protocol::server::Pin& command);
    void handleIncantationStart(const shared::protocol::server::Pic& command);
    void handleIncantationEnd(const shared::protocol::server::Pie& command);
    void handleResourceDropped(const shared::protocol::server::Pdr& command);
    void handleResourceCollected(const shared::protocol::server::Pgt& command);
    void handlePlayerDeath(const shared::protocol::server::Pdi& command);
    void handleEggLaid(const shared::protocol::server::Enw& command);
    void handleEggRemoved(const shared::protocol::server::Ebo& command);
    void handleEggRemoved(const shared::protocol::server::Edi& command);
    void handleExpulsionAnimation(const shared::protocol::server::Pex& command) {}  // TODO
    void handleBroadcastAnimation(const shared::protocol::server::Pbc& command) {}  // TODO
    void handleEggDropAnimation(const shared::protocol::server::Pfk& command) {}    // TODO

    void movePlayers(int serverFrequency, float deltaTime);

  private:
    struct InitialEgg {
        int id{0};
        raylib::rmath::Vector3 position;
    };

    [[nodiscard]] std::optional<std::reference_wrapper<game::Player>> playerById(int id);
    [[nodiscard]] std::optional<std::reference_wrapper<game::Team>> teamForPlayer(int playerId);
    [[nodiscard]] game::Team& ensureTeamExist(const std::string& name);
    [[nodiscard]] raylib::Color nextTeamColor() const;
    [[nodiscard]] static game::Player::cardinalPoint orientationFromProtocol(int orientation);

    void updatePlayerPosition(game::Player& player, Tile3DPosition tilePosition) const;
    void redistributeInitialEggs();
    void removeEgg(int eggId);
    [[nodiscard]] bool wrapPositionIfNeeded(const game::Player& player, Tile3DPosition tilePosition,
                                            raylib::rmath::Vector3& exitPosition) const;

    TileManager& _tileManager;
    std::vector<game::Team> _teams;
    std::vector<Incantation> _activeIncantations;
    std::vector<InitialEgg> _initialEggs;
};
}  // namespace zappy::gui::graphics::scene
