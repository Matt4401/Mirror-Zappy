/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GameHUD
*/

#pragma once

#include <functional>
#include <memory>

#include "events/EventDispatcher.hpp"
#include "rtext/Font.hpp"
#include "ui/UIManager.hpp"
#include "ui/components/UIGridManager.hpp"
#include "ui/menus/EventLogUI.hpp"
#include "ui/menus/PauseMenu.hpp"
#include "ui/menus/PlayerInspectorUI.hpp"
#include "ui/menus/TileInspectorUI.hpp"
#include "ui/menus/WorldControlUI.hpp"

namespace zappy::gui::ui::hud {

class GameHUD {
  public:
    GameHUD(events::EventDispatcher& dispatcher, const std::shared_ptr<raylib::rtext::Font>& font);
    ~GameHUD() = default;

    GameHUD(const GameHUD& other) = delete;
    GameHUD& operator=(const GameHUD& other) = delete;
    GameHUD(GameHUD&& other) = delete;
    GameHUD& operator=(GameHUD&& other) = delete;

    void registerToUIManager(UIManager& uiManager);

    [[nodiscard]] std::shared_ptr<components::UIGridManager> getGridManager() const { return _gridManager; }
    [[nodiscard]] std::shared_ptr<menus::PauseMenu> getPauseMenu() const { return _pauseMenu; }
    [[nodiscard]] std::shared_ptr<menus::PlayerInspectorUI> getPlayerInspector() const { return _playerInspector; }
    [[nodiscard]] std::shared_ptr<menus::WorldControlUI> getWorldControl() const { return _worldControl; }
    [[nodiscard]] std::shared_ptr<menus::EventLogUI> getEventLog() const { return _eventLog; }

  private:
    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    std::shared_ptr<raylib::rtext::Font> _font;
    std::shared_ptr<components::UIGridManager> _gridManager;
    std::shared_ptr<menus::PauseMenu> _pauseMenu;
    std::shared_ptr<menus::PlayerInspectorUI> _playerInspector;
    std::shared_ptr<menus::TileInspectorUI> _tileInspector;
    std::shared_ptr<menus::WorldControlUI> _worldControl;
    std::shared_ptr<menus::EventLogUI> _eventLog;

    static constexpr int WorldControlWidthCols = 26;
    static constexpr int WorldControlX = (components::UIGridManager::GridCols - WorldControlWidthCols) / 2;
    static constexpr int WorldControlHeightCols = 2;
    static constexpr int WorldControlY = components::UIGridManager::GridRows - WorldControlHeightCols;
};

}  // namespace zappy::gui::ui::hud
