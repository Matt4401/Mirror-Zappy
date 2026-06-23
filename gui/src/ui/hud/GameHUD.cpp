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
#include "menus/WorldControlUI.hpp"
#include "rtext/Font.hpp"

namespace zappy::gui::ui::hud {

GameHUD::GameHUD(events::EventDispatcher& dispatcher, const std::shared_ptr<raylib::rtext::Font>& font)
    : _dispatcher(dispatcher), _font(font) {
    _gridManager = std::make_shared<components::UIGridManager>();

    auto sendCommand = [this](const std::string& cmd) { _dispatcher.get().dispatch(events::SendCommand{cmd}); };

    _playerInspector =
        std::make_shared<menus::PlayerInspectorUI>(0.0F, 0.0F, 300.0F, _dispatcher.get(), _font, sendCommand);
    _gridManager->addPanel(_playerInspector, 52, 16, 10, 15);

    _tileInspector =
        std::make_shared<menus::TileInspectorUI>(0.0F, 0.0F, 300.0F, _dispatcher.get(), _font, sendCommand);
    _gridManager->addPanel(_tileInspector, 52, 3, 10, 12);

    _worldControl = std::make_shared<menus::WorldControlUI>(0.0F, 0.0F, _dispatcher.get(), _font, sendCommand);
    _gridManager->addPanel(_worldControl, WorldControlX, WorldControlY, WorldControlWidthCols, WorldControlHeightCols);

    _pauseMenu = std::make_shared<menus::PauseMenu>(_dispatcher.get(), _font);
}

void GameHUD::registerToUIManager(UIManager& uiManager) {
    uiManager.addComponent(_gridManager);
    uiManager.addComponent(_pauseMenu);
}

}  // namespace zappy::gui::ui::hud
