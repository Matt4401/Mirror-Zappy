/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GameHUD
*/

#include "GameHUD.hpp"

#include <memory>
#include <string>
#include <utility>

#include "EventDispatcher.hpp"
#include "UIManager.hpp"
#include "components/UIGridManager.hpp"
#include "events/GuiEvents.hpp"
#include "menus/PauseMenu.hpp"
#include "menus/PlayerInspectorUI.hpp"
#include "menus/TileInspectorUI.hpp"
#include "rtext/Font.hpp"

namespace zappy::gui::ui::hud {

GameHUD::GameHUD(std::shared_ptr<events::EventDispatcher> dispatcher, const std::shared_ptr<raylib::rtext::Font>& font)
    : _dispatcher(std::move(dispatcher)), _font(font) {
    _gridManager = std::make_shared<components::UIGridManager>();

    auto sendCommand = [this](const std::string& cmd) {
        if (_dispatcher) {
            _dispatcher->dispatch(events::SendCommand{cmd});
        }
    };

    _playerInspector = std::make_shared<menus::PlayerInspectorUI>(0.0F, 0.0F, 300.0F, _dispatcher, _font, sendCommand);
    _gridManager->addPanel(_playerInspector, 14, 1, 10, 15);

    _tileInspector = std::make_shared<menus::TileInspectorUI>(0.0F, 0.0F, 300.0F, _dispatcher, _font, sendCommand);
    _gridManager->addPanel(_tileInspector, 1, 1, 10, 12);

    _pauseMenu = std::make_shared<menus::PauseMenu>(_dispatcher, _font);
}

void GameHUD::registerToUIManager(UIManager& uiManager) {
    uiManager.addComponent(_gridManager);
    uiManager.addComponent(_pauseMenu);
}

}  // namespace zappy::gui::ui::hud
