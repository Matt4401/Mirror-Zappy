/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** PauseMenu
*/

#pragma once

#include <functional>
#include <memory>

#include "../IUIComponent.hpp"
#include "../components/UIButton.hpp"
#include "../components/UIPanel.hpp"
#include "events/EventDispatcher.hpp"
#include "rcore/Event.hpp"
#include "rtext/Font.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::menus {

class PauseMenu : public IUIComponent {
  public:
    PauseMenu(std::shared_ptr<events::EventDispatcher> dispatcher, const std::shared_ptr<raylib::rtext::Font>& font);
    ~PauseMenu() override = default;

    PauseMenu(const PauseMenu& other) = delete;
    PauseMenu& operator=(const PauseMenu& other) = delete;
    PauseMenu(PauseMenu&& other) = delete;
    PauseMenu& operator=(PauseMenu&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent(const raylib::rcore::Event& event) override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;

    void setOnExit(std::function<void()> callback);
    void setOnUIConfig(std::function<void()> callback);

  private:
    std::shared_ptr<events::EventDispatcher> _dispatcher;
    std::unique_ptr<components::UIPanel> _backgroundPanel;
    std::unique_ptr<components::UIButton> _resumeBtn;
    std::unique_ptr<components::UIButton> _exitBtn;
    std::unique_ptr<components::UIButton> _uiConfigBtn;
    std::unique_ptr<components::UIButton> _settingsBtn;
    std::unique_ptr<components::UIText> _titleText;

    static constexpr float _panelWidth{400.0F};
    static constexpr float _buttonHeight{60.0F};
    static constexpr float _buttonSpacing{10.0F};

    bool _isVisible{false};
    std::function<void()> _onExit;
    std::function<void()> _onUIConfig;
};

}  // namespace zappy::gui::ui::menus
