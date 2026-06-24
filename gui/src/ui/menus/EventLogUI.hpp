/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** EventLogUI
*/

#pragma once

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Color.hpp"
#include "events/EventDispatcher.hpp"
#include "rtext/Font.hpp"
#include "ui/IUIComponent.hpp"
#include "ui/components/UIGamePanel.hpp"

namespace zappy::gui::ui::menus {

class EventLogUI : public components::UIGamePanel {
  public:
    EventLogUI(float x, float y, float width, float height, events::EventDispatcher& dispatcher,
               std::shared_ptr<raylib::rtext::Font> font);
    ~EventLogUI() override;

    EventLogUI(const EventLogUI& other) = delete;
    EventLogUI& operator=(const EventLogUI& other) = delete;
    EventLogUI(EventLogUI&& other) = delete;
    EventLogUI& operator=(EventLogUI&& other) = delete;

  private:
    void initEventSubscriptions();

    void addLogLine(const std::vector<std::pair<std::string, raylib::Color>>& segments);

    [[nodiscard]] std::string getPlayerName(int playerId) const;
    [[nodiscard]] static std::string getResourceName(int resourceId);
    [[nodiscard]] static raylib::Color getResourceColor(int resourceId);

    events::EventDispatcher& _dispatcher;
    std::shared_ptr<raylib::rtext::Font> _font;
    events::EventDispatcher::EventToken _serverCmdToken{0};
    events::EventDispatcher::EventToken _playerNameToken{0};
    std::unordered_map<int, std::string> _playerNames;

    std::vector<std::shared_ptr<IUIComponent>> _logRows;

    static constexpr size_t MaxLogs = 100;
};

}  // namespace zappy::gui::ui::menus
