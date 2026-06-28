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
#include "SettingsManager.hpp"
#include "SettingsMenu.hpp"
#include "events/EventDispatcher.hpp"
#include "rtext/Font.hpp"
#include "ui/IUIComponent.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIPanel.hpp"

namespace zappy::gui::ui::menus {

class PauseMenu : public IUIComponent {
  public:
    PauseMenu(events::EventDispatcher& dispatcher, AudioManager& audioManager, SettingsManager& settingsManager,
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

    [[nodiscard]] bool isSettingsVisible() const { return _settingsVisible; }

  private:
    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    std::reference_wrapper<AudioManager> _audioManager;
    std::unique_ptr<components::UIPanel> _backgroundPanel;
    std::unique_ptr<components::UIButton> _resumeBtn;
    std::unique_ptr<components::UIButton> _exitBtn;
    std::unique_ptr<components::UIButton> _uiConfigBtn;
    std::unique_ptr<components::UIButton> _settingsBtn;

    std::unique_ptr<SettingsMenu> _settingsMenu;

    static constexpr float _panelWidth{400.0F};
    static constexpr float _buttonHeight{60.0F};
    static constexpr float _buttonSpacing{10.0F};
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

    int _lastScreenWidth{0};
    int _lastScreenHeight{0};
    void recalculateLayout();
};

}  // namespace zappy::gui::ui::menus
