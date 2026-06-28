/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GameOverUI
*/

#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <string>

#include "events/EventDispatcher.hpp"
#include "rtext/Font.hpp"
#include "ui/IUIComponent.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIPanel.hpp"

namespace zappy::gui::ui::menus {

class GameOverUI : public IUIComponent {
  public:
    GameOverUI(events::EventDispatcher& dispatcher, const std::shared_ptr<raylib::rtext::Font>& font);
    ~GameOverUI() override;

    GameOverUI(const GameOverUI& other) = delete;
    GameOverUI& operator=(const GameOverUI& other) = delete;
    GameOverUI(GameOverUI&& other) = delete;
    GameOverUI& operator=(GameOverUI&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent() override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;
    [[nodiscard]] bool isHovered() const override;

    void setOnExit(std::function<void()> callback);
    void setOnSpectate(std::function<void()> callback);

  private:
    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    std::unique_ptr<components::UIPanel> _backgroundPanel;
    std::unique_ptr<components::UIPanel> _modalPanel;
    std::unique_ptr<components::UIButton> _spectateBtn;
    std::unique_ptr<components::UIButton> _exitBtn;

    std::shared_ptr<raylib::rtext::Font> _font;
    std::string _winningTeam;
    std::size_t _segSubscription{0};

    static constexpr float PanelWidth{500.0F};
    static constexpr float PanelHeight{300.0F};
    static constexpr float ButtonWidth{200.0F};
    static constexpr float ButtonHeight{60.0F};
    static constexpr float ButtonSpacing{20.0F};
    static constexpr float HalfRatio{2.0F};
    static constexpr float FontSize{40.0F};
    static constexpr float FontSpacing{1.0F};
    static constexpr float TextYOffset{40.0F};
    static constexpr float ButtonsStartYOffset{110.0F};

    bool _isVisible{false};
    std::function<void()> _onExit;
    std::function<void()> _onSpectate;

    int _lastScreenWidth{0};
    int _lastScreenHeight{0};
    void recalculateLayout();
};

}  // namespace zappy::gui::ui::menus
