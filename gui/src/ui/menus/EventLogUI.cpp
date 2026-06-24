/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** EventLogUI
*/

#include "EventLogUI.hpp"

#include <memory>
#include <string>
#include <type_traits>
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

void EventLogUI::initEventSubscriptions() {
    _playerNameToken = _dispatcher.subscribe<events::PlayerNameChanged>(
        [this](const events::PlayerNameChanged& e) { _playerNames[e.playerId] = e.newName; });

    _serverCmdToken =
        _dispatcher.subscribe<shared::protocol::ServerCommand>([this](const shared::protocol::ServerCommand& cmd) {
            std::visit(
                [this](auto&& arg) {
                    using T = std::decay_t<decltype(arg)>;

                    if constexpr (std::is_same_v<T, shared::protocol::server::Pnw>) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" joined team ", raylib::Color::Black()},
                                    {arg.teamName, raylib::Color::Maroon()}});
                    } else if constexpr (std::is_same_v<T, shared::protocol::server::Pex>) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" used ", raylib::Color::Black()},
                                    {"expulsion", raylib::Color::Red()},
                                    {"!", raylib::Color::Black()}});
                    } else if constexpr (std::is_same_v<T, shared::protocol::server::Pbc>) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" broadcasts: ", raylib::Color::Black()},
                                    {arg.message, raylib::Color::DarkGray()}});
                    } else if constexpr (std::is_same_v<T, shared::protocol::server::Pic>) {
                        addLogLine({{"Incantation started at ", raylib::Color::Black()},
                                    {std::to_string(arg.x) + "," + std::to_string(arg.y), raylib::Color::DarkGray()},
                                    {" for level ", raylib::Color::Black()},
                                    {std::to_string(arg.level), raylib::Color::Maroon()},
                                    {"!", raylib::Color::Black()}});
                    } else if constexpr (std::is_same_v<T, shared::protocol::server::Pie>) {
                        addLogLine({{"Incantation at ", raylib::Color::Black()},
                                    {std::to_string(arg.x) + "," + std::to_string(arg.y), raylib::Color::DarkGray()},
                                    {" finished. Result: ", raylib::Color::Black()},
                                    {std::to_string(arg.incantationResult),
                                     arg.incantationResult ? raylib::Color::DarkGreen() : raylib::Color::Red()}});
                    } else if constexpr (std::is_same_v<T, shared::protocol::server::Pfk>) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" is laying an egg!", raylib::Color::DarkGray()}});
                    } else if constexpr (std::is_same_v<T, shared::protocol::server::Pdr>) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" dropping ", raylib::Color::Black()},
                                    {getResourceName(arg.resourceId), getResourceColor(arg.resourceId)}});
                    } else if constexpr (std::is_same_v<T, shared::protocol::server::Pgt>) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" picking up ", raylib::Color::Black()},
                                    {getResourceName(arg.resourceId), getResourceColor(arg.resourceId)}});
                    } else if constexpr (std::is_same_v<T, shared::protocol::server::Pdi>) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" was starved to death", raylib::Color::Red()}});
                    } else if constexpr (std::is_same_v<T, shared::protocol::server::Enw>) {
                        addLogLine({{getPlayerName(arg.playerId), raylib::Color::Blue()},
                                    {" laid ", raylib::Color::Black()},
                                    {"Egg #" + std::to_string(arg.eggId), raylib::Color::Maroon()},
                                    {"!", raylib::Color::Black()}});
                    } else if constexpr (std::is_same_v<T, shared::protocol::server::Ebo>) {
                        addLogLine({{"Egg #", raylib::Color::Black()},
                                    {std::to_string(arg.eggId), raylib::Color::DarkBrown()},
                                    {" hatched", raylib::Color::DarkGreen()}});
                    } else if constexpr (std::is_same_v<T, shared::protocol::server::Edi>) {
                        addLogLine({{"Egg #", raylib::Color::Black()},
                                    {std::to_string(arg.eggId), raylib::Color::DarkBrown()},
                                    {" died of starvation", raylib::Color::Red()}});
                    } else if constexpr (std::is_same_v<T, shared::protocol::server::Seg>) {
                        addLogLine({{"Game Over! Team ", raylib::Color::Red()},
                                    {arg.teamName, raylib::Color::Maroon()},
                                    {" won!", raylib::Color::Red()}});
                    } else if constexpr (std::is_same_v<T, shared::protocol::server::Smg>) {
                        addLogLine({{"Server: ", raylib::Color::Maroon()}, {arg.message, raylib::Color::DarkGray()}});
                    }
                },
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
