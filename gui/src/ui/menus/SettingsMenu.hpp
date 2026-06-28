/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** SettingsMenu
*/

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "AudioManager.hpp"
#include "SettingsManager.hpp"
#include "events/EventDispatcher.hpp"
#include "rmath/Vector2.hpp"
#include "rtext/Font.hpp"
#include "ui/IUIComponent.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIPanel.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::menus {

class SettingsMenu : public IUIComponent {
  public:
    enum class Tab : std::uint8_t { Audio, Video, Controls, Visibility };

    SettingsMenu(events::EventDispatcher& dispatcher, AudioManager& audioManager, SettingsManager& settingsManager,
                 const std::shared_ptr<raylib::rtext::Font>& font);
    ~SettingsMenu() override = default;

    SettingsMenu(const SettingsMenu& other) = delete;
    SettingsMenu& operator=(const SettingsMenu& other) = delete;
    SettingsMenu(SettingsMenu&& other) = delete;
    SettingsMenu& operator=(SettingsMenu&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent() override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;
    [[nodiscard]] bool isHovered() const override;

    void setOnBack(std::function<void()> callback);

  private:
    void initAudioTab();
    void initVideoTab();
    void initControlsTab();
    void initVisibilityTab();

    void updateVideoModeRevert();
    void switchTab(Tab tab);

    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    std::reference_wrapper<AudioManager> _audioManager;
    std::reference_wrapper<SettingsManager> _settingsManager;
    std::shared_ptr<raylib::rtext::Font> _font;

    raylib::rmath::Vector2 _position;
    raylib::rmath::Vector2 _size;
    bool _isVisible{false};
    Tab _currentTab{Tab::Audio};
    std::function<void()> _onBack;

    std::unique_ptr<components::UIPanel> _backgroundPanel;
    std::unique_ptr<components::UIText> _titleText;
    std::unique_ptr<components::UIButton> _backBtn;

    std::vector<std::string> _tabNames{"Audio", "Video", "Controls", "Visibility"};

    std::vector<std::unique_ptr<IUIComponent>> _audioComponents;
    std::unique_ptr<components::UIText> _masterVolText;
    std::unique_ptr<components::UIText> _musicVolText;
    std::unique_ptr<components::UIText> _sfxVolText;

    std::vector<std::unique_ptr<IUIComponent>> _videoComponents;
    std::unique_ptr<components::UIText> _fovText;
    std::unique_ptr<components::UIText> _fpsText;
    std::unique_ptr<components::UIText> _sensText;
    std::unique_ptr<components::UIText> _modeText;

    std::vector<std::unique_ptr<IUIComponent>> _controlsComponents;
    std::vector<std::unique_ptr<IUIComponent>> _visibilityComponents;
    std::vector<std::unique_ptr<components::UIText>> _visibilityTexts;

    bool _pendingVideoModeChange{false};
    float _videoModeTimer{0.0F};
    int _oldDisplayMode{0};

    static constexpr float TopBarHeight = 60.0F;
    static constexpr float Padding = 20.0F;
    static constexpr float TabButtonWidth = 120.0F;
    static constexpr float TabButtonHeight = 40.0F;
    static constexpr float ContentOffsetY = TopBarHeight + TabButtonHeight + (Padding * 2);
    static constexpr float KeybindBtnWidth = 400.0F;
    static constexpr float KeybindBtnHeight = 30.0F;
    static constexpr float ComponentSpacingY = 40.0F;
    static constexpr float ContentOffsetX = 250.0F;
    static constexpr float LabelOffsetX = 30.0F;
    static constexpr float DefaultMenuWidth = 800.0F;
    static constexpr float DefaultMenuHeight = 600.0F;
    static constexpr float TitleFontSize = 32.0F;
    static constexpr float LabelFontSize = 20.0F;
    static constexpr float BackBtnWidth = 100.0F;
    static constexpr float BackBtnHeight = 40.0F;
    static constexpr float SliderWidth = 300.0F;
    static constexpr float SliderHeight = 20.0F;
    static constexpr float SliderSpacingY = 50.0F;
    static constexpr float ToggleWidth = 150.0F;
    static constexpr float ToggleHeight = 30.0F;
    static constexpr float DropdownWidth = 200.0F;
    static constexpr float DropdownHeight = 30.0F;
    static constexpr float MinFov = 60.0F;
    static constexpr float MaxFov = 120.0F;
    static constexpr float MinFps = 30.0F;
    static constexpr float MaxFps = 240.0F;
    static constexpr float MinSens = 0.1F;
    static constexpr float MaxSens = 5.0F;
    static constexpr float VideoModeRevertTime = 10.0F;
    static constexpr int DefaultWindowWidth = 1280;
    static constexpr int DefaultWindowHeight = 720;
    static constexpr uint8_t TextColorComponent = 30;
};

}  // namespace zappy::gui::ui::menus
