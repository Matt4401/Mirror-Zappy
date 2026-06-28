/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** SettingsMenu
*/

#include "SettingsMenu.hpp"

#include <raylib.h>

#include <cstddef>
#include <functional>
#include <memory>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "AudioManager.hpp"
#include "Color.hpp"
#include "EventDispatcher.hpp"
#include "SettingsManager.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Rectangle.hpp"
#include "rmath/Vector2.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"
#include "ui/IUIComponent.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIDropdown.hpp"
#include "ui/components/UIKeybindButton.hpp"
#include "ui/components/UIPanel.hpp"
#include "ui/components/UISlider.hpp"
#include "ui/components/UIText.hpp"
#include "ui/components/UIToggle.hpp"

namespace zappy::gui::ui::menus {

SettingsMenu::SettingsMenu(events::EventDispatcher& dispatcher, AudioManager& audioManager,
                           SettingsManager& settingsManager, const std::shared_ptr<raylib::rtext::Font>& font)
    : _dispatcher(dispatcher), _audioManager(audioManager), _settingsManager(settingsManager), _font(font) {
    _backgroundPanel = std::make_unique<components::UIPanel>(0, 0, DefaultMenuWidth, DefaultMenuHeight);
    _titleText = std::make_unique<components::UIText>("Settings", font);
    _titleText->setPosition(0, 0);
    _titleText->setFontSize(TitleFontSize);
    _titleText->setColor(raylib::Color(TextColorComponent, TextColorComponent, TextColorComponent, 255));

    _backBtn = std::make_unique<components::UIButton>(0, 0, BackBtnWidth, BackBtnHeight, "Back", font);

    _backBtn->setOnClick([this]() {
        _settingsManager.get().save();
        if (_onBack) {
            _onBack();
        }
    });

    initAudioTab();
    initVideoTab();
    initControlsTab();
    initVisibilityTab();

    setSize(DefaultMenuWidth, DefaultMenuHeight);
    switchTab(Tab::Audio);
}

void SettingsMenu::initAudioTab() {
    auto& settings = _settingsManager.get().getSettings();
    float y = _position.y() + ContentOffsetY;
    float const x = _position.x() + ContentOffsetX;

    auto makeText = [this](const std::string& text, float textY) {
        auto t = std::make_unique<components::UIText>(text, _font);
        t->setPosition(_position.x() + LabelOffsetX, textY);
        t->setFontSize(LabelFontSize);
        t->setColor(raylib::Color(TextColorComponent, TextColorComponent, TextColorComponent, 255));
        return t;
    };

    _masterVolText = makeText("Master Volume: " + std::to_string(static_cast<int>(settings.masterVolume * 100)), y);
    auto masterSlider =
        std::make_unique<components::UISlider>(x, y, SliderWidth, SliderHeight, 0.0F, 1.0F, settings.masterVolume);
    masterSlider->setOnValueChanged([this, &settings](float value) {
        settings.masterVolume = value;
        _masterVolText->setText("Master Volume: " + std::to_string(static_cast<int>(value * 100)));
        if (!settings.muted) {
            _audioManager.get().setMusicVolume(settings.musicVolume * value);
            _audioManager.get().setSoundVolume(settings.sfxVolume * value);
        }
    });
    _audioComponents.push_back(std::move(masterSlider));
    y += SliderSpacingY;

    _musicVolText = makeText("Music Volume: " + std::to_string(static_cast<int>(settings.musicVolume * 100)), y);
    auto musicSlider =
        std::make_unique<components::UISlider>(x, y, SliderWidth, SliderHeight, 0.0F, 1.0F, settings.musicVolume);
    musicSlider->setOnValueChanged([this, &settings](float value) {
        settings.musicVolume = value;
        _musicVolText->setText("Music Volume: " + std::to_string(static_cast<int>(value * 100)));
        if (!settings.muted) {
            _audioManager.get().setMusicVolume(value * settings.masterVolume);
        }
    });
    _audioComponents.push_back(std::move(musicSlider));
    y += SliderSpacingY;

    _sfxVolText = makeText("SFX Volume: " + std::to_string(static_cast<int>(settings.sfxVolume * 100)), y);
    auto sfxSlider =
        std::make_unique<components::UISlider>(x, y, SliderWidth, SliderHeight, 0.0F, 1.0F, settings.sfxVolume);
    sfxSlider->setOnValueChanged([this, &settings](float value) {
        settings.sfxVolume = value;
        _sfxVolText->setText("SFX Volume: " + std::to_string(static_cast<int>(value * 100)));
        if (!settings.muted) {
            _audioManager.get().setSoundVolume(value * settings.masterVolume);
        }
    });
    _audioComponents.push_back(std::move(sfxSlider));
    y += SliderSpacingY;

    auto muteToggle =
        std::make_unique<components::UIToggle>(x, y, ToggleWidth, ToggleHeight, "Mute All", _font, settings.muted);
    muteToggle->setOnToggle([this, &settings](bool state) {
        settings.muted = state;
        if (state) {
            _audioManager.get().setMusicVolume(0.0F);
            _audioManager.get().setSoundVolume(0.0F);
        } else {
            _audioManager.get().setMusicVolume(settings.musicVolume * settings.masterVolume);
            _audioManager.get().setSoundVolume(settings.sfxVolume * settings.masterVolume);
        }
    });
    _audioComponents.push_back(std::move(muteToggle));
}

void SettingsMenu::initVideoTab() {
    auto& settings = _settingsManager.get().getSettings();
    float y = _position.y() + ContentOffsetY;
    float const x = _position.x() + ContentOffsetX;

    auto makeText = [this](const std::string& text, float textY) {
        auto t = std::make_unique<components::UIText>(text, _font);
        t->setPosition(_position.x() + LabelOffsetX, textY);
        t->setFontSize(LabelFontSize);
        t->setColor(raylib::Color(TextColorComponent, TextColorComponent, TextColorComponent, 255));
        return t;
    };

    _fovText = makeText("FOV: " + std::to_string(static_cast<int>(settings.fov)), y);
    auto fovSlider =
        std::make_unique<components::UISlider>(x, y, SliderWidth, SliderHeight, MinFov, MaxFov, settings.fov);
    fovSlider->setOnValueChanged([this, &settings](float value) {
        settings.fov = value;
        _fovText->setText("FOV: " + std::to_string(static_cast<int>(value)));
    });
    _videoComponents.push_back(std::move(fovSlider));
    y += SliderSpacingY;

    _fpsText = makeText("FPS Limit: " + std::to_string(settings.fpsLimit), y);
    auto fpsSlider = std::make_unique<components::UISlider>(x, y, SliderWidth, SliderHeight, MinFps, MaxFps,
                                                            static_cast<float>(settings.fpsLimit));
    fpsSlider->setOnValueChanged([this, &settings](float value) {
        int const fps = static_cast<int>(value);
        settings.fpsLimit = fps;
        _fpsText->setText("FPS Limit: " + std::to_string(fps));
        SetTargetFPS(fps);
    });
    _videoComponents.push_back(std::move(fpsSlider));
    y += SliderSpacingY;

    _sensText = makeText("Sensitivity: " + std::to_string(settings.cameraSensitivity).substr(0, 4), y);
    auto sensSlider = std::make_unique<components::UISlider>(x, y, SliderWidth, SliderHeight, MinSens, MaxSens,
                                                             settings.cameraSensitivity);
    sensSlider->setOnValueChanged([this, &settings](float value) {
        settings.cameraSensitivity = value;
        _sensText->setText("Sensitivity: " + std::to_string(value).substr(0, 4));
    });
    _videoComponents.push_back(std::move(sensSlider));
    y += SliderSpacingY;

    _modeText = makeText("Display Mode:", y);
    std::vector<std::string> const modes = {"Windowed", "Borderless", "Fullscreen"};
    auto modeDropdown = std::make_unique<components::UIDropdown>(x, y, DropdownWidth, DropdownHeight, modes, _font);
    modeDropdown->setSelectedIndex(settings.displayMode);
    modeDropdown->setDirection(components::UIDropdown::Direction::DOWN);
    modeDropdown->setOnSelect([this, &settings, modeDropdownRaw = modeDropdown.get()](const std::string& selection) {
        _oldDisplayMode = settings.displayMode;
        _pendingVideoModeChange = true;
        _videoModeTimer = VideoModeRevertTime;

        if (selection == "Borderless") {
            settings.displayMode = 1;
            if (IsWindowFullscreen()) {
                ToggleFullscreen();
            }
            SetWindowState(FLAG_WINDOW_UNDECORATED);
            SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
            SetWindowPosition(0, 0);
        } else if (selection == "Fullscreen") {
            settings.displayMode = 2;
            ClearWindowState(FLAG_WINDOW_UNDECORATED);
            if (!IsWindowFullscreen()) {
                ToggleFullscreen();
            }
        } else {
            settings.displayMode = 0;
            ClearWindowState(FLAG_WINDOW_UNDECORATED);
            if (IsWindowFullscreen()) {
                ToggleFullscreen();
            }
            SetWindowSize(1280, 720);
            SetWindowPosition((GetMonitorWidth(GetCurrentMonitor()) - 1280) / 2,
                              (GetMonitorHeight(GetCurrentMonitor()) - 720) / 2);
        }
    });
    _videoComponents.push_back(std::move(modeDropdown));
}

void SettingsMenu::initControlsTab() {
    auto& keybinds = _settingsManager.get().getSettings().keybinds;
    float y = _position.y() + ContentOffsetY;
    float const x = _position.x() + 30.0F;

    const std::vector<std::string> orderedActions = {"Move Forward", "Move Backward", "Move Left", "Move Right",
                                                     "Move Up",      "Move Down",     "Sprint",    "Pause",
                                                     "Reset Camera", "Toggle UI"};

    for (const auto& action : orderedActions) {
        if (keybinds.find(action) == keybinds.end()) {
            continue;
        }
        int const key = keybinds.at(action);
        auto keybindBtn =
            std::make_unique<components::UIKeybindButton>(x, y, KeybindBtnWidth, KeybindBtnHeight, action, key, _font);
        keybindBtn->setOnKeyBind(
            [this, action](int newKey) { _settingsManager.get().getSettings().keybinds[action] = newKey; });
        _controlsComponents.push_back(std::move(keybindBtn));
        y += ComponentSpacingY;
    }
}

void SettingsMenu::initVisibilityTab() {
    auto& settings = _settingsManager.get().getSettings();
    float y = _position.y() + ContentOffsetY;
    float const x = _position.x() + ContentOffsetX;

    auto makeToggle = [this, x, &y](const std::string& label, bool state, auto callback) {
        auto t = std::make_unique<components::UIText>(label, _font);
        t->setPosition(_position.x() + LabelOffsetX, y + 5.0F);
        t->setFontSize(LabelFontSize);
        t->setColor(raylib::Color(TextColorComponent, TextColorComponent, TextColorComponent, 255));
        _visibilityTexts.push_back(std::move(t));

        auto toggle = std::make_unique<components::UIToggle>(x, y, DropdownWidth, DropdownHeight, "", _font, state);
        toggle->setOnToggle(callback);
        _visibilityComponents.push_back(std::move(toggle));
        y += ComponentSpacingY;
    };

    makeToggle("Global UI:", settings.showUI,
               [this](bool state) { _settingsManager.get().getSettings().showUI = state; });
    makeToggle("Players:", settings.showPlayers,
               [this](bool state) { _settingsManager.get().getSettings().showPlayers = state; });
    makeToggle("Minerals:", settings.showMinerals,
               [this](bool state) { _settingsManager.get().getSettings().showMinerals = state; });
    makeToggle("Food:", settings.showFood,
               [this](bool state) { _settingsManager.get().getSettings().showFood = state; });
    makeToggle("Grid/Tiles:", settings.showTiles,
               [this](bool state) { _settingsManager.get().getSettings().showTiles = state; });
}

void SettingsMenu::switchTab(Tab tab) { _currentTab = tab; }

void SettingsMenu::setPosition(float x, float y) {
    if (_position.x() == x && _position.y() == y) {
        return;
    }
    _position = {x, y};
    _backgroundPanel->setPosition(x, y);
    _titleText->setPosition(x + Padding, y + Padding);
    _backBtn->setPosition(x + _size.x() - 100 - Padding, y + Padding);

    _audioComponents.clear();
    _videoComponents.clear();
    _controlsComponents.clear();
    _visibilityComponents.clear();
    _visibilityTexts.clear();

    initAudioTab();
    initVideoTab();
    initControlsTab();
    initVisibilityTab();
}

void SettingsMenu::setSize(float width, float height) {
    _size = {width, height};
    _backgroundPanel->setSize(width, height);
    setPosition(_position.x(), _position.y());
}

bool SettingsMenu::isVisible() const { return _isVisible; }

void SettingsMenu::setVisible(bool visible) { _isVisible = visible; }

bool SettingsMenu::isHovered() const {
    if (!_isVisible) {
        return false;
    }
    return _backgroundPanel->isHovered();
}

void SettingsMenu::setOnBack(std::function<void()> callback) { _onBack = std::move(callback); }

void SettingsMenu::draw() {
    if (!_isVisible) {
        return;
    }

    _backgroundPanel->draw();
    _titleText->draw();
    _backBtn->draw();

    float tabX = _position.x() + Padding;
    float const tabY = _position.y() + TopBarHeight;

    raylib::rmath::Rectangle const separatorRect = {.x = _position.x() + Padding,
                                                    .y = tabY + TabButtonHeight - 2.0F,
                                                    .width = _size.x() - (Padding * 2),
                                                    .height = 2.0F};
    raylib::rshapes::Shapes::drawRectangleRec(separatorRect, raylib::Color(150, 150, 150, 255));

    for (std::size_t i = 0; i < _tabNames.size(); ++i) {
        raylib::rmath::Rectangle const tabRect = {
            .x = tabX, .y = tabY, .width = TabButtonWidth, .height = TabButtonHeight};
        bool const isActive = (static_cast<std::size_t>(_currentTab) == i);

        if (isActive) {
            raylib::rshapes::Shapes::drawRectangleRounded(tabRect, 0.2F, 10, raylib::Color(245, 245, 245, 255));
            raylib::rmath::Rectangle const coverRect = {
                .x = tabX, .y = tabY + TabButtonHeight - 2.0F, .width = TabButtonWidth, .height = 2.0F};
            raylib::rshapes::Shapes::drawRectangleRec(coverRect, raylib::Color(245, 245, 245, 255));
        } else {
            raylib::rshapes::Shapes::drawRectangleRounded(tabRect, 0.2F, 10, raylib::Color(220, 220, 220, 180));
        }

        raylib::rmath::Vector2 const textSize = _font->measureTextEx(_tabNames[i], 20.0F, 1.0F);
        float const textX = tabX + ((TabButtonWidth - textSize.x()) / 2.0F);
        float const textY = tabY + ((TabButtonHeight - textSize.y()) / 2.0F);

        raylib::Color const textColor = isActive ? raylib::Color(30, 30, 30, 255) : raylib::Color(120, 120, 120, 255);
        _font->drawTextEx(_tabNames[i], {textX, textY}, 20.0F, 1.0F, textColor);

        if (isActive) {
            raylib::rmath::Rectangle const accentRect = {
                .x = tabX + 10.0F, .y = tabY, .width = TabButtonWidth - 20.0F, .height = 3.0F};
            raylib::rshapes::Shapes::drawRectangleRounded(accentRect, 1.0F, 4, raylib::Color(50, 150, 250, 255));
        }

        tabX += TabButtonWidth + 5.0F;
    }

    if (_currentTab == Tab::Audio) {
        _masterVolText->draw();
        _musicVolText->draw();
        _sfxVolText->draw();
        for (auto& comp : _audioComponents) {
            comp->draw();
        }
    } else if (_currentTab == Tab::Video) {
        _fovText->draw();
        _fpsText->draw();
        _sensText->draw();
        _modeText->draw();
        for (auto& comp : _videoComponents) {
            comp->draw();
        }
    } else if (_currentTab == Tab::Controls) {
        for (auto& comp : _controlsComponents) {
            comp->draw();
        }
    } else if (_currentTab == Tab::Visibility) {
        for (auto& text : _visibilityTexts) {
            text->draw();
        }
        for (auto& comp : _visibilityComponents) {
            comp->draw();
        }
    }
}

void SettingsMenu::update() {
    if (!_isVisible) {
        return;
    }

    if (_pendingVideoModeChange) {
        updateVideoModeRevert();
    }

    _backgroundPanel->update();
    _backBtn->update();

    std::vector<std::unique_ptr<IUIComponent>>* activeComps = nullptr;
    if (_currentTab == Tab::Audio) {
        activeComps = &_audioComponents;
    } else if (_currentTab == Tab::Video) {
        activeComps = &_videoComponents;
    } else if (_currentTab == Tab::Controls) {
        activeComps = &_controlsComponents;
    } else if (_currentTab == Tab::Visibility) {
        activeComps = &_visibilityComponents;
    }

    if (activeComps != nullptr) {
        for (auto& comp : *activeComps) {
            comp->update();
        }
    }
}

void SettingsMenu::handleEvent() {
    if (!_isVisible) {
        return;
    }

    if (_pendingVideoModeChange && _videoModeTimer < 9.5F && raylib::rcore::Event::isMouseButtonReleased(0)) {
        _pendingVideoModeChange = false;
    }

    _backBtn->handleEvent();

    if (raylib::rcore::Event::isMouseButtonPressed(0)) {
        float const mouseX = raylib::rcore::Event::getMousePositionStatic().x();
        float const mouseY = raylib::rcore::Event::getMousePositionStatic().y();

        float tabX = _position.x() + Padding;
        float const tabY = _position.y() + TopBarHeight;

        for (std::size_t i = 0; i < _tabNames.size(); ++i) {
            if (mouseX >= tabX && mouseX <= tabX + TabButtonWidth && mouseY >= tabY &&
                mouseY <= tabY + TabButtonHeight) {
                switchTab(static_cast<Tab>(i));
                break;
            }
            tabX += TabButtonWidth + 5.0F;
        }
    }

    std::vector<std::unique_ptr<IUIComponent>>* activeComps = nullptr;
    if (_currentTab == Tab::Audio) {
        activeComps = &_audioComponents;
    } else if (_currentTab == Tab::Video) {
        activeComps = &_videoComponents;
    } else if (_currentTab == Tab::Controls) {
        activeComps = &_controlsComponents;
    } else if (_currentTab == Tab::Visibility) {
        activeComps = &_visibilityComponents;
    }

    if (activeComps != nullptr) {
        for (auto& activeComp : std::ranges::reverse_view(*activeComps)) {
            activeComp->handleEvent();
        }
    }
}

void SettingsMenu::updateVideoModeRevert() {
    _videoModeTimer -= raylib::rcore::Window::frameTime();
    if (_videoModeTimer <= 0.0F) {
        _pendingVideoModeChange = false;
        auto& settings = _settingsManager.get().getSettings();
        settings.displayMode = _oldDisplayMode;

        if (_oldDisplayMode == 1) {
            if (IsWindowFullscreen()) {
                ToggleFullscreen();
            }
            SetWindowState(FLAG_WINDOW_UNDECORATED);
            SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
            SetWindowPosition(0, 0);
        } else if (_oldDisplayMode == 2) {
            ClearWindowState(FLAG_WINDOW_UNDECORATED);
            if (!IsWindowFullscreen()) {
                ToggleFullscreen();
            }
        } else {
            ClearWindowState(FLAG_WINDOW_UNDECORATED);
            if (IsWindowFullscreen()) {
                ToggleFullscreen();
            }
            SetWindowSize(DefaultWindowWidth, DefaultWindowHeight);
            SetWindowPosition((GetMonitorWidth(GetCurrentMonitor()) - DefaultWindowWidth) / 2,
                              (GetMonitorHeight(GetCurrentMonitor()) - DefaultWindowHeight) / 2);
        }

        for (auto& comp : _videoComponents) {
            if (auto* dropdown = dynamic_cast<components::UIDropdown*>(comp.get())) {
                dropdown->setSelectedIndex(_oldDisplayMode);
            }
        }
    }
}

}  // namespace zappy::gui::ui::menus
