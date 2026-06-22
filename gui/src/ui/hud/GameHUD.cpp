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
#include "Font.hpp"
#include "PauseMenu.hpp"
#include "PlayerInspectorUI.hpp"
#include "TileInspectorUI.hpp"
#include "UIGridManager.hpp"
#include "UIManager.hpp"
#include "events/GuiEvents.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIGamePanel.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::hud {

GameHUD::GameHUD(std::shared_ptr<events::EventDispatcher> dispatcher, const std::shared_ptr<raylib::rtext::Font>& font)
    : _dispatcher(std::move(dispatcher)), _font(font) {
    _gridManager = std::make_shared<components::UIGridManager>();

    auto demoPanel = std::make_shared<components::UIGamePanel>(0, 0, 0, 0, "Player Info");
    auto btn = std::make_shared<components::UIButton>(100.0F, 120.0F, 200.0F, 40.0F, "Click me!", _font);
    demoPanel->addComponent(btn);

    auto demoPanel2 = std::make_shared<components::UIGamePanel>(0, 0, 0, 0, "Inventory");
    auto text2 = std::make_shared<components::UIText>("Empty", _font);
    text2->setPosition(40.0F, 200.0F);
    demoPanel2->addComponent(text2);

    _gridManager->addPanel(demoPanel, 1, 1, 12, 18);
    _gridManager->addPanel(demoPanel2, 1, 19, 12, 8);

    auto sendCommand = [this](const std::string& cmd) {
        if (_dispatcher) {
            _dispatcher->dispatch(events::SendCommand{cmd});
        }
    };

    _playerInspector = std::make_shared<menus::PlayerInspectorUI>(0.0F, 0.0F, 300.0F, _dispatcher, _font, sendCommand);
    _gridManager->addPanel(_playerInspector, 14, 1, 10, 15);

    _tileInspector = std::make_shared<menus::TileInspectorUI>(0.0F, 0.0F, 300.0F, _dispatcher, _font, sendCommand);
    _gridManager->addPanel(_tileInspector, 14, 1, 10, 15);

    _pauseMenu = std::make_shared<menus::PauseMenu>(_dispatcher, _font);
}

void GameHUD::registerToUIManager(UIManager& uiManager) {
    uiManager.addComponent(_gridManager);
    uiManager.addComponent(_pauseMenu);
}

}  // namespace zappy::gui::ui::hud
