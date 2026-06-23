/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** WorldManager
*/

#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Color.hpp"
#include "Tile3D.hpp"
#include "events/EventDispatcher.hpp"
#include "gui/src/game/ItemBag.hpp"
#include "gui/src/game/Player.hpp"
#include "gui/src/game/Team.hpp"
#include "gui/src/game/components/Deraumere.hpp"
#include "gui/src/game/components/Food.hpp"
#include "gui/src/game/components/IObject.hpp"
#include "gui/src/game/components/Linemate.hpp"
#include "gui/src/game/components/Mendiane.hpp"
#include "gui/src/game/components/Phiras.hpp"
#include "gui/src/game/components/Sibur.hpp"
#include "gui/src/game/components/Thystame.hpp"
#include "protocol/Commands.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::graphics::scene {
struct Incantation {
    int x{0};
    int y{0};
    int level{0};
    std::vector<int> playerIds;
};

class WorldManager {
  public:
    static constexpr auto DefaultPlayerOffsetY = 1.4F;
    static constexpr std::array<raylib::Color, 8> TeamColors = {
        raylib::Color::Red(),    raylib::Color::Blue(),   raylib::Color::Green(),   raylib::Color::Yellow(),
        raylib::Color::Purple(), raylib::Color::Orange(), raylib::Color::SkyBlue(), raylib::Color::Pink(),
    };
    explicit WorldManager(std::shared_ptr<events::EventDispatcher> dispatcher);
    ~WorldManager();
    WorldManager(const WorldManager& other) = delete;
    WorldManager& operator=(const WorldManager& other) = delete;
    WorldManager(WorldManager&& other) = delete;
    WorldManager& operator=(WorldManager&& other) = delete;

    [[nodiscard]] int width() const { return _width; }
    [[nodiscard]] int height() const { return _height; }
    [[nodiscard]] const std::vector<Tile3D>& tiles() const { return _tiles; }
    [[nodiscard]] const std::vector<game::Team>& teams() const { return _teams; }
    [[nodiscard]] int timeUnit() const { return _timeUnit; }
    [[nodiscard]] const std::optional<std::string>& winningTeam() const { return _winningTeam; }
    [[nodiscard]] const std::string& lastServerMessage() const { return _lastServerMessage; }
    [[nodiscard]] const std::vector<Incantation>& activeIncantations() const { return _activeIncantations; }
    [[nodiscard]] std::optional<std::reference_wrapper<const Tile3D>> tileAt(int x, int y) const;
    [[nodiscard]] std::optional<std::reference_wrapper<const game::Player>> playerById(int id) const;
    void updatePlayerName(int id, const std::string& name);

  private:
    using ResourceFactory = std::function<std::unique_ptr<game::IObject>(const raylib::rmath::Vector3&)>;

    void handleMapSize(const shared::protocol::server::Msz& command);
    void handleTileContent(const shared::protocol::server::Bct& command);
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
    void handleTimeUnit(const shared::protocol::server::Sgt& command);
    void handleTimeUnit(const shared::protocol::server::Sst& command);
    void handleGameEnd(const shared::protocol::server::Seg& command);
    void handleServerMessage(const shared::protocol::server::Smg& command);

    [[nodiscard]] bool contains(int x, int y) const;
    [[nodiscard]] std::optional<std::reference_wrapper<Tile3D>> mutableTileAt(int x, int y);
    [[nodiscard]] std::optional<std::reference_wrapper<game::Player>> mutablePlayerById(int id);
    [[nodiscard]] std::optional<std::reference_wrapper<game::Team>> teamForPlayer(int playerId);
    [[nodiscard]] raylib::rmath::Vector3 tilePosition(int x, int y) const;
    [[nodiscard]] game::Team& ensureTeamExist(const std::string& name);
    [[nodiscard]] raylib::Color nextTeamColor() const;
    [[nodiscard]] static game::Player::cardinalPoint orientationFromProtocol(int orientation);
    [[nodiscard]] static std::optional<std::string_view> resourceName(int resourceId);
    [[nodiscard]] std::unique_ptr<game::IObject> makeResource(int resourceId,
                                                              const raylib::rmath::Vector3& position);

    void updatePlayerPosition(game::Player& player, int x, int y);
    void replaceResources(game::ItemBag& bag, const raylib::rmath::Vector3& position,
                                 const std::array<int, 7>& quantities);
    void addResource(game::ItemBag& bag, const raylib::rmath::Vector3& position, int resourceId,
                            std::size_t quantity = 1);
    static void removeResource(game::ItemBag& bag, int resourceId, std::size_t quantity = 1);
    void removeEgg(int eggId);

    template <typename EventType>
    void subscribe(const std::function<void(const EventType&)>& callback) {
        const auto token = _dispatcher->subscribe<EventType>(callback);
        _unsubscribers.emplace_back([this, token]() { _dispatcher->unsubscribe<EventType>(token); });
    }

    std::shared_ptr<events::EventDispatcher> _dispatcher;
    std::vector<std::function<void()>> _unsubscribers;
    int _width{0};
    int _height{0};
    int _timeUnit{100};
    std::vector<Tile3D> _tiles;
    std::vector<game::Team> _teams;
    std::vector<Incantation> _activeIncantations;
    std::optional<std::string> _winningTeam;
    std::string _lastServerMessage;

    const std::unordered_map<int, ResourceFactory> _resourceFactories = {
        {0, [](const auto& resourcePosition) { return std::make_unique<game::Food>(resourcePosition); }},
        {1, [](const auto& resourcePosition) { return std::make_unique<game::Linemate>(resourcePosition); }},
        {2, [](const auto& resourcePosition) { return std::make_unique<game::Deraumere>(resourcePosition); }},
        {3, [](const auto& resourcePosition) { return std::make_unique<game::Sibur>(resourcePosition); }},
        {4, [](const auto& resourcePosition) { return std::make_unique<game::Mendiane>(resourcePosition); }},
        {5, [](const auto& resourcePosition) { return std::make_unique<game::Phiras>(resourcePosition); }},
        {6, [](const auto& resourcePosition) { return std::make_unique<game::Thystame>(resourcePosition); }},
    };
};
}  // namespace zappy::gui::graphics::scene
