/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** PauseMenu
*/

#include "PauseMenu.hpp"

#include <functional>
#include <memory>
#include <utility>

#include "Color.hpp"
#include "EventDispatcher.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rtext/Font.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIPanel.hpp"

namespace zappy::gui::ui::menus {

PauseMenu::PauseMenu(std::shared_ptr<events::EventDispatcher> dispatcher,
                     const std::shared_ptr<raylib::rtext::Font>& font)
    : _dispatcher(std::move(dispatcher)) {
    int const screenWidth = raylib::rcore::Window::screenWidth();
    int const screenHeight = raylib::rcore::Window::screenHeight();

    _backgroundPanel = std::make_unique<components::UIPanel>(
        0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight), raylib::Color(0, 0, 0, 180));
    _backgroundPanel->setRounded(false);

    float const startX = (static_cast<float>(screenWidth) - _panelWidth) / 2.0F;
    float const startY = (static_cast<float>(screenHeight) - (_buttonHeight * 4 + _buttonSpacing * 3)) / 2.0F;
    _resumeBtn =
        std::make_unique<components::UIButton>(startX, startY, _panelWidth, _buttonHeight, "Back to Game", font);
    _settingsBtn = std::make_unique<components::UIButton>(startX, startY + _buttonHeight + _buttonSpacing, _panelWidth,
                                                          _buttonHeight, "Settings", font);
    _uiConfigBtn = std::make_unique<components::UIButton>(startX, startY + ((_buttonHeight + _buttonSpacing) * 2),
                                                          _panelWidth, _buttonHeight, "UI Configuration", font);
    _exitBtn = std::make_unique<components::UIButton>(startX, startY + ((_buttonHeight + _buttonSpacing) * 3),
                                                      _panelWidth, _buttonHeight, "Save and Quit to Title", font);

    _resumeBtn->setOnClick([this]() {
        this->setVisible(false);
        raylib::rcore::Window::disableCursor();
    });

    _uiConfigBtn->setOnClick([this]() {
        if (this->_onUIConfig) {
            this->_onUIConfig();
        }
    });

    _exitBtn->setOnClick([this]() {
        if (this->_onExit) {
            this->_onExit();
        }
    });
}

void PauseMenu::draw() {
    if (!_isVisible) {
        return;
    }
    _backgroundPanel->draw();
    _resumeBtn->draw();
    _exitBtn->draw();
    _uiConfigBtn->draw();
    _settingsBtn->draw();
}

void PauseMenu::update() {
    if (!_isVisible) {
        return;
    }
    _backgroundPanel->update();
    _resumeBtn->update();
    _exitBtn->update();
    _uiConfigBtn->update();
    _settingsBtn->update();
}

void PauseMenu::handleEvent(const raylib::rcore::Event& event) {
    if (!_isVisible) {
        return;
    }
    _resumeBtn->handleEvent(event);
    _exitBtn->handleEvent(event);
    _uiConfigBtn->handleEvent(event);
    _settingsBtn->handleEvent(event);
}

void PauseMenu::setPosition(float /*x*/, float /*y*/) {}
void PauseMenu::setSize(float /*width*/, float /*height*/) {}

bool PauseMenu::isVisible() const { return _isVisible; }
void PauseMenu::setVisible(bool visible) { _isVisible = visible; }

void PauseMenu::setOnExit(std::function<void()> callback) { _onExit = std::move(callback); }
void PauseMenu::setOnUIConfig(std::function<void()> callback) { _onUIConfig = std::move(callback); }

}  // namespace zappy::gui::ui::menus
