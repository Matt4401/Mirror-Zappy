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
#include <string>
#include <utility>

#include "AudioManager.hpp"
#include "Color.hpp"
#include "EventDispatcher.hpp"
#include "rcore/Window.hpp"
#include "rtext/Font.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIPanel.hpp"
#include "ui/components/UISlider.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::menus {

PauseMenu::PauseMenu(events::EventDispatcher& dispatcher, AudioManager& audioManager,
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
    float const settingsX = (static_cast<float>(screenWidth) - _settingsPanelWidth) / _halfRatio;
    float const settingsY = (static_cast<float>(screenHeight) - _settingsPanelHeight) / _halfRatio;

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

    _settingsPanel = std::make_unique<components::UIPanel>(settingsX, settingsY, _settingsPanelWidth,
                                                           _settingsPanelHeight, raylib::Color(224, 224, 224, 255));
    _settingsTitleText = std::make_unique<components::UIText>("Settings", font);
    _settingsTitleText->setFontSize(_settingsTitleFontSize);
    _settingsTitleText->setPosition(settingsX + _settingsTitleOffsetX, settingsY + _settingsTitleOffsetY);

    _musicVolumeText = std::make_unique<components::UIText>("Music volume", font);
    _musicVolumeText->setFontSize(_settingsLabelFontSize);
    _musicVolumeText->setPosition(settingsX + _musicLabelOffsetX, settingsY + _musicLabelOffsetY);

    _soundVolumeText = std::make_unique<components::UIText>("Sound volume", font);
    _soundVolumeText->setFontSize(_settingsLabelFontSize);
    _soundVolumeText->setPosition(settingsX + _soundLabelOffsetX, settingsY + _soundLabelOffsetY);

    _musicVolumeSlider = std::make_unique<components::UISlider>(
        settingsX + _settingsSliderOffsetX, settingsY + _musicSliderOffsetY, _sliderWidth, _sliderHeight,
        _volumeSliderMin, _volumeSliderMax, _audioManager.get().musicVolume());
    _soundVolumeSlider = std::make_unique<components::UISlider>(
        settingsX + _settingsSliderOffsetX, settingsY + _soundSliderOffsetY, _sliderWidth, _sliderHeight,
        _volumeSliderMin, _volumeSliderMax, _audioManager.get().soundVolume());
    _settingsBackBtn = std::make_unique<components::UIButton>(
        settingsX + ((_settingsPanelWidth - _settingsBackButtonWidth) / _halfRatio),
        settingsY + _settingsBackButtonOffsetY, _settingsBackButtonWidth, _buttonHeight, "Back", font);

    _resumeBtn->setOnClick([this]() {
        this->setVisible(false);
        raylib::rcore::Window::disableCursor();
    });

    _settingsBtn->setOnClick([this]() { this->_settingsVisible = true; });

    _musicVolumeSlider->setOnValueChanged([this](const float value) {
        this->_audioManager.get().setMusicVolume(value);
        this->updateVolumeLabels();
    });
    _soundVolumeSlider->setOnValueChanged([this](const float value) {
        this->_audioManager.get().setSoundVolume(value);
        this->updateVolumeLabels();
    });
    _settingsBackBtn->setOnClick([this]() { this->_settingsVisible = false; });

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
    updateVolumeLabels();
}

void PauseMenu::draw() {
    if (!_isVisible) {
        return;
    }
    _backgroundPanel->draw();
    if (_settingsVisible) {
        drawSettingsPanel();
        return;
    }
    drawMainMenu();
}

void PauseMenu::drawMainMenu() {
    _resumeBtn->draw();
    _exitBtn->draw();
    _uiConfigBtn->draw();
    _settingsBtn->draw();
}

void PauseMenu::drawSettingsPanel() {
    _settingsPanel->draw();
    _settingsTitleText->draw();
    _musicVolumeText->draw();
    _soundVolumeText->draw();
    _musicVolumeSlider->draw();
    _soundVolumeSlider->draw();
    _settingsBackBtn->draw();
}

void PauseMenu::update() {
    if (!_isVisible) {
        return;
    }
    _backgroundPanel->update();
    if (_settingsVisible) {
        updateSettingsPanel();
        return;
    }
    updateMainMenu();
}

void PauseMenu::updateMainMenu() {
    _resumeBtn->update();
    _exitBtn->update();
    _uiConfigBtn->update();
    _settingsBtn->update();
}

void PauseMenu::updateSettingsPanel() {
    _settingsPanel->update();
    _settingsTitleText->update();
    _musicVolumeText->update();
    _soundVolumeText->update();
    _musicVolumeSlider->update();
    _soundVolumeSlider->update();
    _settingsBackBtn->update();
}

void PauseMenu::handleEvent() {
    if (!_isVisible) {
        return;
    }
    if (_settingsVisible) {
        handleSettingsPanelEvent();
        return;
    }
    handleMainMenuEvent();
}

void PauseMenu::handleMainMenuEvent() {
    _resumeBtn->handleEvent();
    _exitBtn->handleEvent();
    _uiConfigBtn->handleEvent();
    _settingsBtn->handleEvent();
}

void PauseMenu::handleSettingsPanelEvent() {
    _musicVolumeSlider->handleEvent();
    _soundVolumeSlider->handleEvent();
    _settingsBackBtn->handleEvent();
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
        return (_backgroundPanel && _backgroundPanel->isHovered()) || (_settingsPanel && _settingsPanel->isHovered()) ||
               (_musicVolumeSlider && _musicVolumeSlider->isHovered()) ||
               (_soundVolumeSlider && _soundVolumeSlider->isHovered()) ||
               (_settingsBackBtn && _settingsBackBtn->isHovered());
    }
    return (_backgroundPanel && _backgroundPanel->isHovered()) || (_resumeBtn && _resumeBtn->isHovered()) ||
           (_exitBtn && _exitBtn->isHovered()) || (_uiConfigBtn && _uiConfigBtn->isHovered()) ||
           (_settingsBtn && _settingsBtn->isHovered());
}

void PauseMenu::setOnExit(std::function<void()> callback) { _onExit = std::move(callback); }
void PauseMenu::setOnUIConfig(std::function<void()> callback) { _onUIConfig = std::move(callback); }

void PauseMenu::updateVolumeLabels() {
    auto const musicPercent = static_cast<int>(std::lround(_audioManager.get().musicVolume() * 100.0F));
    auto const soundPercent = static_cast<int>(std::lround(_audioManager.get().soundVolume() * 100.0F));
    _musicVolumeText->setText("Music: " + std::to_string(musicPercent) + "%");
    _soundVolumeText->setText("Sounds: " + std::to_string(soundPercent) + "%");
}

}  // namespace zappy::gui::ui::menus
