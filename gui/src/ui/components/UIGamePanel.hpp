/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIGamePanel
*/

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Color.hpp"
#include "UIPanel.hpp"
#include "UIText.hpp"
#include "rcore/Event.hpp"
#include "rmath/Vector2.hpp"
#include "ui/IUIComponent.hpp"

namespace zappy::gui::ui::components {

class UIGamePanel : public IUIComponent {
  public:
    UIGamePanel(float x, float y, float width, float height, const std::string& title);
    UIGamePanel(float x, float y, float width, float height, const std::string& title,
                const raylib::Color& primaryColor, const raylib::Color& secondaryColor,
                const raylib::Color& titleColor);
    ~UIGamePanel() override = default;

    UIGamePanel(const UIGamePanel& other) = delete;
    UIGamePanel& operator=(const UIGamePanel& other) = delete;
    UIGamePanel(UIGamePanel&& other) = delete;
    UIGamePanel& operator=(UIGamePanel&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent(const raylib::rcore::Event& event) override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;

    void addComponent(const std::shared_ptr<IUIComponent>& component);
    void removeComponent(const std::shared_ptr<IUIComponent>& component);

    void setConfigMode(bool configMode) { _isConfigMode = configMode; }
    void updateChildrenLayout();

    void setNextPanel(const std::shared_ptr<UIGamePanel>& panel, float gapPixels = 0.0F);
    [[nodiscard]] raylib::rmath::Vector2 getPosition() const { return _position; }

  private:
    void updateTextPosition();

    raylib::rmath::Vector2 _position;
    raylib::rmath::Vector2 _size;
    bool _isVisible{true};

    std::shared_ptr<UIPanel> _mainPanel;
    std::shared_ptr<UIPanel> _contentPanel;
    std::shared_ptr<UIText> _titleText;
    std::vector<std::shared_ptr<IUIComponent>> _contentChildren;

    std::weak_ptr<UIGamePanel> _nextPanel;
    float _nextPanelGap{0.0F};

    bool _isCollapsed{false};
    bool _isConfigMode{false};
    float _expandedHeight;
    float _currentHeight;

    float _scrollOffset{0.0F};
    float _maxScroll{0.0F};

    static constexpr int MouseLeftButton = 0;
    static constexpr float DefaultHeaderHeight = 40.0F;
    static constexpr float Padding = 10.0F;
    static constexpr float LerpSpeedMultiplier = 15.0F;
    static constexpr int DefaultFontSize = 20;
    static constexpr float TextSpacing = 2.0F;
    static constexpr float ScrollSpeed = 30.0F;

    static const raylib::Color PrimaryColor;
    static const raylib::Color SecondaryColor;
    static const raylib::Color TitleColor;
};

}  // namespace zappy::gui::ui::components
