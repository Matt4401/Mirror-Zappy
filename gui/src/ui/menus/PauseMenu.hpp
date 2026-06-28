/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** PauseMenu
*/

#pragma once

#include <functional>
#include <memory>

#include "AudioManager.hpp"
#include "events/EventDispatcher.hpp"
#include "rtext/Font.hpp"
#include "ui/IUIComponent.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIPanel.hpp"
#include "ui/components/UISlider.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::menus {

class PauseMenu : public IUIComponent {
  public:
    PauseMenu(events::EventDispatcher& dispatcher, AudioManager& audioManager,
              const std::shared_ptr<raylib::rtext::Font>& font);
    ~PauseMenu() override = default;

    PauseMenu(const PauseMenu& other) = delete;
    PauseMenu& operator=(const PauseMenu& other) = delete;
    PauseMenu(PauseMenu&& other) = delete;
    PauseMenu& operator=(PauseMenu&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent() override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;
    [[nodiscard]] bool isHovered() const override;

    void setOnExit(std::function<void()> callback);
    void setOnUIConfig(std::function<void()> callback);
    void setOnResume(std::function<void()> callback);

  private:
    void drawMainMenu();
    void drawSettingsPanel();
    void updateMainMenu();
    void updateSettingsPanel();
    void handleMainMenuEvent();
    void handleSettingsPanelEvent();
    void updateVolumeLabels();

    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    std::reference_wrapper<AudioManager> _audioManager;
    std::unique_ptr<components::UIPanel> _backgroundPanel;
    std::unique_ptr<components::UIPanel> _settingsPanel;
    std::unique_ptr<components::UIButton> _resumeBtn;
    std::unique_ptr<components::UIButton> _exitBtn;
    std::unique_ptr<components::UIButton> _uiConfigBtn;
    std::unique_ptr<components::UIButton> _settingsBtn;
    std::unique_ptr<components::UIButton> _settingsBackBtn;
    std::unique_ptr<components::UIText> _titleText;
    std::unique_ptr<components::UIText> _settingsTitleText;
    std::unique_ptr<components::UIText> _musicVolumeText;
    std::unique_ptr<components::UIText> _soundVolumeText;
    std::unique_ptr<components::UISlider> _musicVolumeSlider;
    std::unique_ptr<components::UISlider> _soundVolumeSlider;

    static constexpr float _panelWidth{400.0F};
    static constexpr float _settingsPanelWidth{520.0F};
    static constexpr float _settingsPanelHeight{320.0F};
    static constexpr float _buttonHeight{60.0F};
    static constexpr float _sliderWidth{300.0F};
    static constexpr float _sliderHeight{24.0F};
    static constexpr float _buttonSpacing{10.0F};
    static constexpr float _settingsTitleFontSize{32.0F};
    static constexpr float _settingsLabelFontSize{24.0F};
    static constexpr float _settingsTitleOffsetX{24.0F};
    static constexpr float _settingsTitleOffsetY{22.0F};
    static constexpr float _musicLabelOffsetX{20.0F};
    static constexpr float _musicLabelOffsetY{100.0F};
    static constexpr float _soundLabelOffsetX{10.0F};
    static constexpr float _soundLabelOffsetY{170.0F};
    static constexpr float _settingsSliderOffsetX{190.0F};
    static constexpr float _musicSliderOffsetY{100.0F};
    static constexpr float _soundSliderOffsetY{170.0F};
    static constexpr float _settingsBackButtonWidth{180.0F};
    static constexpr float _settingsBackButtonOffsetY{238.0F};
    static constexpr float _volumeSliderMin{0.0F};
    static constexpr float _volumeSliderMax{1.0F};
    static constexpr float _halfRatio{2.0F};
    static constexpr int _mainButtonCount{4};
    static constexpr int _mainButtonGapCount{3};
    static constexpr int _uiConfigButtonIndex{2};
    static constexpr int _exitButtonIndex{3};
    static constexpr int _backgroundX{0};
    static constexpr int _backgroundY{0};

    bool _isVisible{false};
    bool _settingsVisible{false};
    std::function<void()> _onExit;
    std::function<void()> _onUIConfig;
    std::function<void()> _onResume;
};

}  // namespace zappy::gui::ui::menus
