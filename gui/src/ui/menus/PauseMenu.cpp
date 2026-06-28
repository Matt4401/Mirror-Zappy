/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** PauseMenu
*/

#include "PauseMenu.hpp"

#include <cmath>
#include <functional>
#include <memory>
#include <utility>

#include "AudioManager.hpp"
#include "Color.hpp"
#include "EventDispatcher.hpp"
#include "SettingsManager.hpp"
#include "SettingsMenu.hpp"
#include "rcore/Window.hpp"
#include "rtext/Font.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIPanel.hpp"

namespace zappy::gui::ui::menus {

PauseMenu::PauseMenu(events::EventDispatcher& dispatcher, AudioManager& audioManager, SettingsManager& settingsManager,
                     const std::shared_ptr<raylib::rtext::Font>& font)
    : _dispatcher(dispatcher), _audioManager(audioManager) {
    int const screenWidth = raylib::rcore::Window::screenWidth();
    int const screenHeight = raylib::rcore::Window::screenHeight();

    _backgroundPanel =
        std::make_unique<components::UIPanel>(_backgroundX, _backgroundY, static_cast<float>(screenWidth),
                                              static_cast<float>(screenHeight), raylib::Color(0, 0, 0, 180));
    _backgroundPanel->setRounded(false);

    float const startX = (static_cast<float>(screenWidth) - _panelWidth) / _halfRatio;
    float const startY =
        (static_cast<float>(screenHeight) - ((_buttonHeight * static_cast<float>(_mainButtonCount)) +
                                             (_buttonSpacing * static_cast<float>(_mainButtonGapCount)))) /
        _halfRatio;

    _resumeBtn =
        std::make_unique<components::UIButton>(startX, startY, _panelWidth, _buttonHeight, "Back to Game", font);
    _settingsBtn = std::make_unique<components::UIButton>(startX, startY + _buttonHeight + _buttonSpacing, _panelWidth,
                                                          _buttonHeight, "Settings", font);
    _uiConfigBtn = std::make_unique<components::UIButton>(
        startX, startY + ((_buttonHeight + _buttonSpacing) * static_cast<float>(_uiConfigButtonIndex)), _panelWidth,
        _buttonHeight, "UI Configuration", font);
    _exitBtn = std::make_unique<components::UIButton>(
        startX, startY + ((_buttonHeight + _buttonSpacing) * static_cast<float>(_exitButtonIndex)), _panelWidth,
        _buttonHeight, "Save and Quit to Title", font);

    _settingsMenu = std::make_unique<SettingsMenu>(_dispatcher.get(), _audioManager.get(), settingsManager, font);
    _settingsMenu->setPosition((static_cast<float>(screenWidth) - 800.0F) / 2.0F,
                               (static_cast<float>(screenHeight) - 600.0F) / 2.0F);
    _settingsMenu->setOnBack([this]() {
        _settingsVisible = false;
        _settingsMenu->setVisible(false);
    });

    _lastScreenWidth = screenWidth;
    _lastScreenHeight = screenHeight;

    _resumeBtn->setOnClick([this]() {
        if (_onResume) {
            _onResume();
        } else {
            this->setVisible(false);
            raylib::rcore::Window::disableCursor();
        }
    });

    _settingsBtn->setOnClick([this]() {
        this->_settingsVisible = true;
        this->_settingsMenu->setVisible(true);
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
    if (_settingsVisible) {
        _settingsMenu->draw();
        return;
    }
    _resumeBtn->draw();
    _exitBtn->draw();
    _uiConfigBtn->draw();
    _settingsBtn->draw();
}

void PauseMenu::update() {
    if (!_isVisible) {
        return;
    }

    int const screenWidth = raylib::rcore::Window::screenWidth();
    int const screenHeight = raylib::rcore::Window::screenHeight();
    if (screenWidth != _lastScreenWidth || screenHeight != _lastScreenHeight) {
        _lastScreenWidth = screenWidth;
        _lastScreenHeight = screenHeight;
        recalculateLayout();
    }

    _backgroundPanel->update();
    if (_settingsVisible) {
        _settingsMenu->update();
        return;
    }
    _resumeBtn->update();
    _exitBtn->update();
    _uiConfigBtn->update();
    _settingsBtn->update();
}

void PauseMenu::recalculateLayout() {
    _backgroundPanel->setSize(static_cast<float>(_lastScreenWidth), static_cast<float>(_lastScreenHeight));

    float const startX = (static_cast<float>(_lastScreenWidth) - _panelWidth) / _halfRatio;
    float const startY =
        (static_cast<float>(_lastScreenHeight) - ((_buttonHeight * static_cast<float>(_mainButtonCount)) +
                                                  (_buttonSpacing * static_cast<float>(_mainButtonGapCount)))) /
        _halfRatio;

    _resumeBtn->setPosition(startX, startY);
    _settingsBtn->setPosition(startX, startY + _buttonHeight + _buttonSpacing);
    _uiConfigBtn->setPosition(startX,
                              startY + ((_buttonHeight + _buttonSpacing) * static_cast<float>(_uiConfigButtonIndex)));
    _exitBtn->setPosition(startX, startY + ((_buttonHeight + _buttonSpacing) * static_cast<float>(_exitButtonIndex)));

    _settingsMenu->setPosition((static_cast<float>(_lastScreenWidth) - 800.0F) / 2.0F,
                               (static_cast<float>(_lastScreenHeight) - 600.0F) / 2.0F);
}

void PauseMenu::handleEvent() {
    if (!_isVisible) {
        return;
    }
    if (_settingsVisible) {
        _settingsMenu->handleEvent();
        return;
    }
    _resumeBtn->handleEvent();
    _exitBtn->handleEvent();
    _uiConfigBtn->handleEvent();
    _settingsBtn->handleEvent();
}

void PauseMenu::setPosition(float /*x*/, float /*y*/) {}
void PauseMenu::setSize(float /*width*/, float /*height*/) {}

bool PauseMenu::isVisible() const { return _isVisible; }

void PauseMenu::setVisible(bool visible) {
    _isVisible = visible;
    if (!visible) {
        _settingsVisible = false;
    }
}

bool PauseMenu::isHovered() const {
    if (!_isVisible) {
        return false;
    }
    if (_settingsVisible) {
        return _settingsMenu->isHovered() || (_backgroundPanel && _backgroundPanel->isHovered());
    }
    return (_backgroundPanel && _backgroundPanel->isHovered()) || (_resumeBtn && _resumeBtn->isHovered()) ||
           (_exitBtn && _exitBtn->isHovered()) || (_uiConfigBtn && _uiConfigBtn->isHovered()) ||
           (_settingsBtn && _settingsBtn->isHovered());
}

void PauseMenu::setOnExit(std::function<void()> callback) { _onExit = std::move(callback); }
void PauseMenu::setOnUIConfig(std::function<void()> callback) { _onUIConfig = std::move(callback); }
void PauseMenu::setOnResume(std::function<void()> callback) { _onResume = std::move(callback); }

}  // namespace zappy::gui::ui::menus
