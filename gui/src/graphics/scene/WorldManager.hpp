/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** WorldManager
*/

#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "PlayerManager.hpp"
#include "Tile3D.hpp"
#include "TileManager.hpp"
#include "events/EventDispatcher.hpp"
#include "gui/src/game/Player.hpp"
#include "gui/src/game/Team.hpp"
#include "protocol/Commands.hpp"

namespace zappy::gui::graphics::scene {
class WorldManager {
  public:
    explicit WorldManager(std::shared_ptr<events::EventDispatcher> dispatcher);
    ~WorldManager();
    WorldManager(const WorldManager& other) = delete;
    WorldManager& operator=(const WorldManager& other) = delete;
    WorldManager(WorldManager&& other) = delete;
    WorldManager& operator=(WorldManager&& other) = delete;

    [[nodiscard]] int width() const { return _tileManager.width(); }
    [[nodiscard]] int height() const { return _tileManager.height(); }
    [[nodiscard]] const std::vector<Tile3D>& tiles() const { return _tileManager.tiles(); }
    [[nodiscard]] const std::vector<game::Team>& teams() const { return _playerManager.teams(); }
    [[nodiscard]] int timeUnit() const { return _timeUnit; }
    [[nodiscard]] const std::optional<std::string>& winningTeam() const { return _winningTeam; }
    [[nodiscard]] const std::string& lastServerMessage() const { return _lastServerMessage; }
    [[nodiscard]] const std::vector<Incantation>& activeIncantations() const {
        return _playerManager.activeIncantations();
    }
    [[nodiscard]] std::optional<std::reference_wrapper<const Tile3D>> tileAt(int x, int y) const {
        return _tileManager.tileAt(x, y);
    }
    [[nodiscard]] std::optional<std::reference_wrapper<const game::Player>> playerById(int id) const {
        return _playerManager.playerById(id);
    }
    void updatePlayerName(int id, const std::string& name) { _playerManager.updatePlayerName(id, name); }

  private:
    void handleTimeUnit(const shared::protocol::server::Sgt& command);
    void handleTimeUnit(const shared::protocol::server::Sst& command);
    void handleGameEnd(const shared::protocol::server::Seg& command);
    void handleServerMessage(const shared::protocol::server::Smg& command);
    void handleUnknownCommand(const shared::protocol::ServerCommand& command) {}  // TODO
    void handleWrongParam(const shared::protocol::server::Sbp& command) {}        // TODO

    void initMapSubscriptions();
    void initPlayerSubscriptions();
    void initResourceSubscriptions();
    void initEggSubscriptions();
    void initGameSubscriptions();

    template <typename Cmd, typename InstanceType>
    void registerHandler(InstanceType* instance, void (InstanceType::*handler)(const Cmd&)) {
        subscribe<Cmd>([instance, handler](const Cmd& cmd) { (instance->*handler)(cmd); });
    }

    template <typename EventType>
    void subscribe(const std::function<void(const EventType&)>& callback) {
        const auto token = _dispatcher->subscribe<EventType>(callback);
        _unsubscribers.emplace_back([this, token]() { _dispatcher->unsubscribe<EventType>(token); });
    }

    std::shared_ptr<events::EventDispatcher> _dispatcher;
    std::vector<std::function<void()>> _unsubscribers;
    TileManager _tileManager;
    PlayerManager _playerManager{_tileManager};
    int _timeUnit{100};
    std::optional<std::string> _winningTeam;
    std::string _lastServerMessage;
};
}  // namespace zappy::gui::graphics::scene
