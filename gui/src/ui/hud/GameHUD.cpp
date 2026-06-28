/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GameHUD
*/

#include "GameHUD.hpp"

#include <functional>
#include <memory>
#include <string>

#include "AudioManager.hpp"
#include "EventDispatcher.hpp"
#include "SettingsManager.hpp"
#include "UIManager.hpp"
#include "components/UIButton.hpp"
#include "components/UICompass.hpp"
#include "components/UIGridManager.hpp"
#include "events/GuiEvents.hpp"
#include "menus/EventLogUI.hpp"
#include "menus/GameOverUI.hpp"
#include "menus/GlobalStatsUI.hpp"
#include "menus/PauseMenu.hpp"
#include "menus/PlayerInspectorUI.hpp"
#include "menus/TileInspectorUI.hpp"
#include "menus/WorldControlUI.hpp"
#include "rcore/Camera.hpp"
#include "rtext/Font.hpp"

namespace zappy::gui::ui::hud {

std::function<void(const std::string&)> GameHUD::makeSendCommand(events::EventDispatcher& dispatcher) {
    return [&dispatcher](const std::string& cmd) { dispatcher.dispatch(events::SendCommand{cmd}); };
}

GameHUD::GameHUD(events::EventDispatcher& dispatcher, AudioManager& audioManager, SettingsManager& settingsManager,
                 const std::shared_ptr<raylib::rtext::Font>& font, raylib::rcore::Camera& camera)
    : _dispatcher(dispatcher),
      _font(font),
      _gridManager(std::make_shared<components::UIGridManager>()),
      _compass(std::make_shared<components::UICompass>(0.0F, 0.0F, 0.0F, 0.0F, camera, _font)),
      _pauseMenu(std::make_shared<menus::PauseMenu>(_dispatcher.get(), audioManager, settingsManager, _font)),
      _gameOverUI(std::make_shared<menus::GameOverUI>(_dispatcher.get(), _font)),
      _playerInspector(std::make_shared<menus::PlayerInspectorUI>(0.0F, 0.0F, 300.0F, _dispatcher.get(), _font,
                                                                  makeSendCommand(_dispatcher.get()))),
      _tileInspector(std::make_shared<menus::TileInspectorUI>(0.0F, 0.0F, 300.0F, _dispatcher.get(), _font,
                                                              makeSendCommand(_dispatcher.get()))),
      _worldControl(std::make_shared<menus::WorldControlUI>(0.0F, 0.0F, _dispatcher.get(), _font,
                                                            makeSendCommand(_dispatcher.get()))),
      _eventLog(std::make_shared<menus::EventLogUI>(0.0F, 0.0F, 400.0F, 300.0F, _dispatcher.get(), _font)),
      _globalStats(std::make_shared<menus::GlobalStatsUI>(0.0F, 0.0F, 400.0F, 300.0F, _dispatcher.get(), _font)) {
    auto* const audio = &audioManager;
    components::UIButton::setClickSoundHandler([audio]() { audio->playSound("button_click"); });

    _gridManager->addPanel(_playerInspector, 52, 16, 10, 15);

    _gridManager->addPanel(_tileInspector, 52, 3, 10, 12);

    _gridManager->addPanel(_worldControl, WorldControlX, WorldControlY, WorldControlWidthCols, WorldControlHeightCols);

    _gridManager->addPanel(_eventLog, 2, 16, 13, 15);

    _gridManager->addPanel(_globalStats, 2, 2, 13, 13);
}

GameHUD::~GameHUD() { components::UIButton::setClickSoundHandler(nullptr); }

void GameHUD::registerToUIManager(UIManager& uiManager) {
    uiManager.addComponent(_compass);
    uiManager.addComponent(_gridManager);
    uiManager.addComponent(_pauseMenu);
    uiManager.addComponent(_gameOverUI);
}

}  // namespace zappy::gui::ui::hud
