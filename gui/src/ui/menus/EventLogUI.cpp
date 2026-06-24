/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** EventLogUI
*/

#include "EventLogUI.hpp"

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "Color.hpp"
#include "events/EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "protocol/Commands.hpp"
#include "rtext/Font.hpp"
#include "ui/components/EventLogRow.hpp"
#include "ui/components/UIGamePanel.hpp"

namespace zappy::gui::ui::menus {

EventLogUI::EventLogUI(float x, float y, float width, float height, events::EventDispatcher& dispatcher,
                       std::shared_ptr<raylib::rtext::Font> font)
    : components::UIGamePanel(x, y, width, height, "Log Events"), _dispatcher(dispatcher), _font(std::move(font)) {
    initEventSubscriptions();
    addLogLine({{"System: ", raylib::Color::Maroon()}, {"Event Log Initialized.", raylib::Color::DarkGray()}});
}

EventLogUI::~EventLogUI() {
    if (_serverCmdToken != 0) {
        _dispatcher.unsubscribe<shared::protocol::ServerCommand>(_serverCmdToken);
    }
    if (_playerNameToken != 0) {
        _dispatcher.unsubscribe<events::PlayerNameChanged>(_playerNameToken);
    }
}

std::string EventLogUI::getPlayerName(int playerId) const {
    if (auto it = _playerNames.find(playerId); it != _playerNames.end()) {
        return it->second;
    }
    return "Player #" + std::to_string(playerId);
}

namespace {
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
}  // namespace

void EventLogUI::initEventSubscriptions() {
    _playerNameToken = _dispatcher.subscribe<events::PlayerNameChanged>(
        [this](const events::PlayerNameChanged& e) { _playerNames[e.playerId] = e.newName; });

    _serverCmdToken =
        _dispatcher.subscribe<shared::protocol::ServerCommand>([this](const shared::protocol::ServerCommand& cmd) {
            std::visit(
                overloaded{
                    [this](const shared::protocol::server::Pnw& arg) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" joined team ", raylib::Color::Black()},
                                    {arg.teamName, raylib::Color::Maroon()}});
                    },
                    [this](const shared::protocol::server::Pex& arg) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" used ", raylib::Color::Black()},
                                    {"expulsion", raylib::Color::Red()},
                                    {"!", raylib::Color::Black()}});
                    },
                    [this](const shared::protocol::server::Pbc& arg) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" broadcasts: ", raylib::Color::Black()},
                                    {arg.message, raylib::Color::DarkGray()}});
                    },
                    [this](const shared::protocol::server::Pic& arg) {
                        addLogLine({{"Incantation started at ", raylib::Color::Black()},
                                    {std::to_string(arg.x) + "," + std::to_string(arg.y), raylib::Color::DarkGray()},
                                    {" for level ", raylib::Color::Black()},
                                    {std::to_string(arg.level), raylib::Color::Maroon()},
                                    {"!", raylib::Color::Black()}});
                    },
                    [this](const shared::protocol::server::Pie& arg) {
                        addLogLine({{"Incantation at ", raylib::Color::Black()},
                                    {std::to_string(arg.x) + "," + std::to_string(arg.y), raylib::Color::DarkGray()},
                                    {" finished. Result: ", raylib::Color::Black()},
                                    {arg.incantationResult ? "success" : "failure",
                                     arg.incantationResult ? raylib::Color::DarkGreen() : raylib::Color::Red()}});
                    },
                    [this](const shared::protocol::server::Pfk& arg) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" is laying an egg !", raylib::Color::DarkGray()}});
                    },
                    [this](const shared::protocol::server::Pdr& arg) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" dropping ", raylib::Color::Black()},
                                    {getResourceName(arg.resourceId), getResourceColor(arg.resourceId)}});
                    },
                    [this](const shared::protocol::server::Pgt& arg) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" picking up ", raylib::Color::Black()},
                                    {getResourceName(arg.resourceId), getResourceColor(arg.resourceId)}});
                    },
                    [this](const shared::protocol::server::Pdi& arg) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" was starved to death", raylib::Color::Red()}});
                    },
                    [this](const shared::protocol::server::Enw& arg) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" laid ", raylib::Color::Black()},
                                    {"Egg #" + std::to_string(arg.eggId), raylib::Color::Maroon()},
                                    {" !", raylib::Color::Black()}});
                    },
                    [this](const shared::protocol::server::Ebo& arg) {
                        addLogLine({{"Egg #", raylib::Color::Black()},
                                    {std::to_string(arg.eggId), raylib::Color::DarkBrown()},
                                    {" hatched", raylib::Color::DarkGreen()}});
                    },
                    [this](const shared::protocol::server::Edi& arg) {
                        addLogLine({{"Egg #", raylib::Color::Black()},
                                    {std::to_string(arg.eggId), raylib::Color::DarkBrown()},
                                    {" died of starvation", raylib::Color::Red()}});
                    },
                    [this](const shared::protocol::server::Seg& arg) {
                        addLogLine({{"Game Over! Team ", raylib::Color::Red()},
                                    {arg.teamName, raylib::Color::Maroon()},
                                    {" won!", raylib::Color::Red()}});
                    },
                    [this](const shared::protocol::server::Smg& arg) {
                        addLogLine({{"Server: ", raylib::Color::Maroon()}, {arg.message, raylib::Color::DarkGray()}});
                    },
                    [](const auto& /*unused*/) {}},
                cmd);
        });
}

void EventLogUI::addLogLine(const std::vector<std::pair<std::string, raylib::Color>>& segments) {
    auto row = std::make_shared<components::EventLogRow>(segments, _font);

    _logRows.push_back(row);
    addComponent(row);

    if (_logRows.size() > MaxLogs) {
        auto oldest = _logRows.front();
        _logRows.erase(_logRows.begin());
        removeComponent(oldest);
    }
    scrollToBottom();
}

std::string EventLogUI::getResourceName(int resourceId) {
    switch (resourceId) {
        case 0:
            return "Food";
        case 1:
            return "Linemate";
        case 2:
            return "Deraumere";
        case 3:
            return "Sibur";
        case 4:
            return "Mendiane";
        case 5:
            return "Phiras";
        case 6:
            return "Thystame";
        default:
            return "Unknown";
    }
}

raylib::Color EventLogUI::getResourceColor(int resourceId) {
    switch (resourceId) {
        case 0:
            return raylib::Color::Red();  // Food
        case 1:
            return raylib::Color::DarkGreen();  // Linemate
        case 2:
            return raylib::Color::DarkBlue();  // Deraumere
        case 3:
            return raylib::Color::DarkPurple();  // Sibur
        case 4:
            return raylib::Color::Maroon();  // Mendiane
        case 5:
            return raylib::Color::Brown();  // Phiras
        case 6:
            return raylib::Color::Black();  // Thystame
        default:
            return raylib::Color::DarkGray();
    }
}

}  // namespace zappy::gui::ui::menus
