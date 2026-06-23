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
    void handleEvent() override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;
    [[nodiscard]] bool isHovered() const override;

    void addComponent(const std::shared_ptr<IUIComponent>& component);
    void addHeaderComponent(const std::shared_ptr<IUIComponent>& component);
    void removeComponent(const std::shared_ptr<IUIComponent>& component);

    virtual void setConfigMode(bool configMode) {
        if (_isConfigMode == configMode) {
            return;
        }
        _isConfigMode = configMode;
        if (_isConfigMode) {
            _wasVisibleBeforeConfig = _isVisible;
            setVisible(true);
            _isCollapsed = false;
        } else {
            setVisible(_wasVisibleBeforeConfig);
        }
    }
    [[nodiscard]] bool isConfigMode() const { return _isConfigMode; }
    void setCustomLayout(bool custom) { _customLayout = custom; }
    void setDragged(bool dragged) { _isDragged = dragged; }
    [[nodiscard]] bool isDragged() const { return _isDragged; }

    void setResizable(bool resizable) { _isResizable = resizable; }
    [[nodiscard]] bool isResizable() const { return _isResizable; }

    void updateChildrenLayout();

    [[nodiscard]] bool isCollapsed() const { return _isCollapsed; }
    void setCollapsed(bool collapsed) { _isCollapsed = collapsed; }

    void setTitle(const std::string& title);

    void setNextPanel(const std::shared_ptr<UIGamePanel>& panel, float gapPixels = 0.0F);
    [[nodiscard]] raylib::rmath::Vector2 getPosition() const { return _position; }
    [[nodiscard]] raylib::rmath::Vector2 getSize() const { return _size; }
    [[nodiscard]] float getCurrentHeight() const { return _currentHeight; }
    [[nodiscard]] static constexpr float getHeaderHeight() { return DefaultHeaderHeight; }

  protected:
    void setBasePosition(float x, float y) {
        _position.setX(x);
        _position.setY(y);
    }
    void setBaseSize(float width, float height) {
        _size.setX(width);
        _size.setY(height);
    }

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
    bool _customLayout = false;
    bool _isDragged = false;
    bool _isResizable = true;
    bool _wasVisibleBeforeConfig{true};
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
    static constexpr std::string DefaultFontName = "Minecraft";

    static const raylib::Color PrimaryColor;
    static const raylib::Color SecondaryColor;
    static const raylib::Color TitleColor;
};

}  // namespace zappy::gui::ui::components
