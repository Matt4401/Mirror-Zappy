/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GlobalStatsUI
*/

#include "GlobalStatsUI.hpp"

#include <algorithm>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "Color.hpp"
#include "EventDispatcher.hpp"
#include "components/UIGamePanel.hpp"
#include "components/UIText.hpp"
#include "graphics/scene/PlayerManager.hpp"
#include "protocol/Commands.hpp"
#include "rcore/Window.hpp"
#include "rmath/Rectangle.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"

namespace zappy::gui::ui::menus {

GlobalStatsUI::GlobalStatsUI(float x, float y, float width, float height, events::EventDispatcher& dispatcher,
                             std::shared_ptr<raylib::rtext::Font> font)
    : components::UIGamePanel(x, y, width, height, "Global Statistics"),
      _dispatcher(dispatcher),
      _font(std::move(font)) {
    _totalPlayersText = std::make_shared<components::UIText>("Players: 0", _font);
    _totalPlayersText->setFontSize(TitleFontSize);
    _totalPlayersText->setColor(raylib::Color::Black());

    _totalEggsText = std::make_shared<components::UIText>("Eggs: 0", _font);
    _totalEggsText->setFontSize(TitleFontSize);
    _totalEggsText->setColor(raylib::Color::Black());

    setCustomLayout(true);
    initEventSubscriptions();
}

GlobalStatsUI::~GlobalStatsUI() {
    for (const auto& token : _eventTokens) {
        _dispatcher.get().unsubscribe<shared::protocol::server::Tna>(token);
        _dispatcher.get().unsubscribe<shared::protocol::server::Pnw>(token);
        _dispatcher.get().unsubscribe<shared::protocol::server::Pdi>(token);
        _dispatcher.get().unsubscribe<shared::protocol::server::Plv>(token);
        _dispatcher.get().unsubscribe<shared::protocol::server::Enw>(token);
        _dispatcher.get().unsubscribe<shared::protocol::server::Ebo>(token);
        _dispatcher.get().unsubscribe<shared::protocol::server::Edi>(token);
    }
}

void GlobalStatsUI::onTnaReceived(const shared::protocol::server::Tna& cmd) { ensureTeamExists(cmd.teamName); }

void GlobalStatsUI::onPnwReceived(const shared::protocol::server::Pnw& cmd) {
    if (_players.contains(cmd.playerId)) {
        return;
    }
    _players[cmd.playerId] = PlayerInfo{.teamName = cmd.teamName, .level = cmd.level};
    _totalPlayers = static_cast<int>(_players.size());

    TeamStat& team = ensureTeamExists(cmd.teamName);
    team.population++;
    if (cmd.level >= VictoryLevel) {
        team.level8Count++;
    }
}

void GlobalStatsUI::onPlvReceived(const shared::protocol::server::Plv& cmd) {
    if (auto it = _players.find(cmd.playerId); it != _players.end()) {
        if (cmd.level >= VictoryLevel && it->second.level < VictoryLevel) {
            TeamStat& team = ensureTeamExists(it->second.teamName);
            team.level8Count++;
        }
        it->second.level = cmd.level;
    }
}

void GlobalStatsUI::onPdiReceived(const shared::protocol::server::Pdi& cmd) {
    if (auto it = _players.find(cmd.playerId); it != _players.end()) {
        TeamStat& team = ensureTeamExists(it->second.teamName);
        team.population--;
        if (it->second.level >= VictoryLevel) {
            team.level8Count--;
        }
        _players.erase(it);
        _totalPlayers--;
    }
}

void GlobalStatsUI::onEnwReceived(const shared::protocol::server::Enw& cmd) {
    if (_eggs.insert(cmd.eggId).second) {
        _totalEggs = static_cast<int>(_eggs.size());
    }
}

void GlobalStatsUI::onEboReceived(const shared::protocol::server::Ebo& cmd) {
    if (_eggs.erase(cmd.eggId) > 0) {
        _totalEggs = static_cast<int>(_eggs.size());
    }
}

void GlobalStatsUI::onEdiReceived(const shared::protocol::server::Edi& cmd) {
    if (_eggs.erase(cmd.eggId) > 0) {
        _totalEggs = static_cast<int>(_eggs.size());
    }
}

void GlobalStatsUI::initEventSubscriptions() {
    auto tnaToken = _dispatcher.get().subscribe<shared::protocol::server::Tna>(
        [this](const shared::protocol::server::Tna& cmd) { onTnaReceived(cmd); });
    _eventTokens.push_back(tnaToken);

    auto pnwToken = _dispatcher.get().subscribe<shared::protocol::server::Pnw>(
        [this](const shared::protocol::server::Pnw& cmd) { onPnwReceived(cmd); });
    _eventTokens.push_back(pnwToken);

    auto plvToken = _dispatcher.get().subscribe<shared::protocol::server::Plv>(
        [this](const shared::protocol::server::Plv& cmd) { onPlvReceived(cmd); });
    _eventTokens.push_back(plvToken);

    auto pdiToken = _dispatcher.get().subscribe<shared::protocol::server::Pdi>(
        [this](const shared::protocol::server::Pdi& cmd) { onPdiReceived(cmd); });
    _eventTokens.push_back(pdiToken);

    auto enwToken = _dispatcher.get().subscribe<shared::protocol::server::Enw>(
        [this](const shared::protocol::server::Enw& cmd) { onEnwReceived(cmd); });
    _eventTokens.push_back(enwToken);

    auto eboToken = _dispatcher.get().subscribe<shared::protocol::server::Ebo>(
        [this](const shared::protocol::server::Ebo& cmd) { onEboReceived(cmd); });
    _eventTokens.push_back(eboToken);

    auto ediToken = _dispatcher.get().subscribe<shared::protocol::server::Edi>(
        [this](const shared::protocol::server::Edi& cmd) { onEdiReceived(cmd); });
    _eventTokens.push_back(ediToken);
}

GlobalStatsUI::TeamStat& GlobalStatsUI::ensureTeamExists(const std::string& teamName) {
    auto it = std::ranges::find_if(_teams, [&teamName](const TeamStat& t) { return t.name == teamName; });
    if (it != _teams.end()) {
        return *it;
    }
    _teams.push_back(TeamStat{.name = teamName, .color = nextTeamColor(), .population = 0, .level8Count = 0});
    return _teams.back();
}

raylib::Color GlobalStatsUI::nextTeamColor() const {
    const auto& colors = zappy::gui::graphics::scene::PlayerManager::TeamColors;
    return colors.at(_teams.size() % colors.size());
}

void GlobalStatsUI::update() {
    components::UIGamePanel::update();

    if (!isVisible()) {
        return;
    }

    _totalPlayersText->setText("Players: " + std::to_string(_totalPlayers));
    _totalEggsText->setText("Eggs: " + std::to_string(_totalEggs));

    std::ranges::sort(_teams, [](const TeamStat& a, const TeamStat& b) {
        if (a.level8Count != b.level8Count) {
            return a.level8Count > b.level8Count;
        }
        return a.population > b.population;
    });
}

void GlobalStatsUI::draw() {
    if (!isVisible()) {
        return;
    }

    components::UIGamePanel::draw();

    float const padding = components::UIGamePanel::getPadding();
    float const headerH = components::UIGamePanel::getHeaderHeight() + 20.0F;
    float const currentH = this->getCurrentHeight();
    float const panelW = this->getSize().x();
    float const startX = this->getPosition().x();

    float const innerX = startX + padding;
    float const innerY = this->getPosition().y() + headerH;
    float const innerW = panelW - (2.0F * padding);
    float const innerH = currentH - headerH - padding;

    raylib::rcore::Window::beginScissorMode(static_cast<int>(innerX), static_cast<int>(innerY),
                                            static_cast<int>(innerW), static_cast<int>(innerH));

    float currentY = innerY - getScrollOffset();

    drawGlobalHeader(innerX, innerW, currentY);
    drawTeamList(innerX, innerW, currentY);

    float const contentBottomWithoutScroll = currentY + getScrollOffset();
    float const innerBottom = this->getPosition().y() + currentH - padding;
    float const calculatedMaxScroll = std::max(0.0F, contentBottomWithoutScroll - innerBottom + MaxScrollPadding);
    setMaxScroll(calculatedMaxScroll);

    raylib::rcore::Window::endScissorMode();
}

void GlobalStatsUI::drawGlobalHeader(float innerX, float innerW, float& currentY) {
    if (_totalPlayersText) {
        _totalPlayersText->setPosition(innerX + BoxPadding, currentY + BoxPadding);
        _totalPlayersText->draw();
    }
    if (_totalEggsText) {
        float const eggX = innerX + innerW - _totalEggsText->getWidth() - BoxPadding;
        _totalEggsText->setPosition(eggX, currentY + BoxPadding);
        _totalEggsText->draw();
    }

    currentY += GlobalHeaderHeight;

    raylib::rshapes::Shapes::drawLine(static_cast<int>(innerX + BoxPadding), static_cast<int>(currentY),
                                      static_cast<int>(innerX + innerW - BoxPadding), static_cast<int>(currentY),
                                      raylib::Color::Gray());
    currentY += SeparatorOffsetY;
}

void GlobalStatsUI::drawTeamList(float innerX, float innerW, float& currentY) {
    for (const auto& team : _teams) {
        float const rowY = currentY;

        raylib::rmath::Rectangle const colorBox{
            .x = innerX + BoxPadding, .y = rowY + ColorBoxOffsetY, .width = ColorBoxSize, .height = ColorBoxSize};
        raylib::rshapes::Shapes::drawRectangleRec(colorBox, team.color);
        raylib::rshapes::Shapes::drawRectangleLinesEx(colorBox, ColorBoxThickness, raylib::Color::Black());

        components::UIText nameText(team.name, _font);
        nameText.setFontSize(DefaultFontSize);
        nameText.setColor(raylib::Color::Black());
        nameText.setPosition(colorBox.x + colorBox.width + NameTextOffsetX, rowY + TextOffsetY);
        nameText.draw();

        std::stringstream progSs;
        progSs << team.level8Count << "/" << RequiredVictoryPlayers << " (Lvl " << VictoryLevel << ")";
        components::UIText progText(progSs.str(), _font);
        progText.setFontSize(DefaultFontSize);
        if (team.level8Count >= RequiredVictoryPlayers) {
            progText.setColor(raylib::Color::Green());
        } else {
            progText.setColor(raylib::Color::Black());
        }
        float const progX = innerX + innerW - progText.getWidth() - BoxPadding;
        progText.setPosition(progX, rowY + TextOffsetY);
        progText.draw();

        std::stringstream popSs;
        popSs << "Pop: " << team.population;
        components::UIText popText(popSs.str(), _font);
        popText.setFontSize(DefaultFontSize);
        popText.setColor(raylib::Color::DarkGray());
        float const popX = progX - popText.getWidth() - PopTextOffsetFromProgX;
        popText.setPosition(popX, rowY + TextOffsetY);
        popText.draw();

        currentY += RowHeight;
    }
}

}  // namespace zappy::gui::ui::menus
