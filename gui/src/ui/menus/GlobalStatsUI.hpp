/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GlobalStatsUI
*/

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Color.hpp"
#include "events/EventDispatcher.hpp"
#include "protocol/Commands.hpp"
#include "rtext/Font.hpp"
#include "ui/components/UIGamePanel.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::menus {

class GlobalStatsUI : public components::UIGamePanel {
  public:
    GlobalStatsUI(float x, float y, float width, float height, events::EventDispatcher& dispatcher,
                  std::shared_ptr<raylib::rtext::Font> font);
    ~GlobalStatsUI() override;

    GlobalStatsUI(const GlobalStatsUI& other) = delete;
    GlobalStatsUI& operator=(const GlobalStatsUI& other) = delete;
    GlobalStatsUI(GlobalStatsUI&& other) = delete;
    GlobalStatsUI& operator=(GlobalStatsUI&& other) = delete;

    void draw() override;
    void update() override;

  private:
    struct TeamStat {
        std::string name;
        raylib::Color color;
        int population{0};
        int level8Count{0};
    };

    struct PlayerInfo {
        std::string teamName;
        int level{1};
    };

    void initEventSubscriptions();

    void onTnaReceived(const shared::protocol::server::Tna& cmd);
    void onPnwReceived(const shared::protocol::server::Pnw& cmd);
    void onPlvReceived(const shared::protocol::server::Plv& cmd);
    void onPdiReceived(const shared::protocol::server::Pdi& cmd);
    void onEnwReceived(const shared::protocol::server::Enw& cmd);
    void onEboReceived(const shared::protocol::server::Ebo& cmd);
    void onEdiReceived(const shared::protocol::server::Edi& cmd);

    TeamStat& ensureTeamExists(const std::string& teamName);
    raylib::Color nextTeamColor() const;

    void drawGlobalHeader(float innerX, float innerW, float& currentY);
    void drawTeamList(float innerX, float innerW, float& currentY);

    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    std::shared_ptr<raylib::rtext::Font> _font;
    std::vector<events::EventDispatcher::EventToken> _eventTokens;

    std::unordered_map<int, PlayerInfo> _players;
    std::unordered_set<int> _eggs;

    std::vector<TeamStat> _teams;

    std::shared_ptr<components::UIText> _totalPlayersText;
    std::shared_ptr<components::UIText> _totalEggsText;

    int _totalPlayers{0};
    int _totalEggs{0};

    static constexpr int DefaultFontSize = 16;
    static constexpr int TitleFontSize = 20;
    static constexpr float BoxPadding = 10.0F;
    static constexpr float RowHeight = 35.0F;
    static constexpr float GlobalHeaderHeight = 40.0F;
    static constexpr int VictoryLevel = 8;
    static constexpr int RequiredVictoryPlayers = 6;

    static constexpr float SeparatorOffsetY = 10.0F;
    static constexpr float ColorBoxOffsetY = 5.0F;
    static constexpr float ColorBoxSize = 20.0F;
    static constexpr float ColorBoxThickness = 1.0F;
    static constexpr float NameTextOffsetX = 10.0F;
    static constexpr float TextOffsetY = 8.0F;
    static constexpr float PopTextOffsetFromProgX = 15.0F;
    static constexpr float MaxScrollPadding = 20.0F;
};

}  // namespace zappy::gui::ui::menus
