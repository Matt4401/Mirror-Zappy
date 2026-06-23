/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIGridManager
*/

#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "UIGamePanel.hpp"
#include "rmath/Vector2.hpp"
#include "ui/IUIComponent.hpp"

namespace zappy::gui::ui::components {

struct GridRect {
    int x;
    int y;
    int w;
    int h;
};

class UIGridManager : public IUIComponent {
  public:
    UIGridManager();
    ~UIGridManager() override = default;

    UIGridManager(const UIGridManager& other) = delete;
    UIGridManager& operator=(const UIGridManager& other) = delete;
    UIGridManager(UIGridManager&& other) = delete;
    UIGridManager& operator=(UIGridManager&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent() override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;
    [[nodiscard]] bool isHovered() const override;

    void addPanel(const std::shared_ptr<UIGamePanel>& panel, int gridX, int gridY, int gridW, int gridH);
    void setConfigMode(bool configMode);
    [[nodiscard]] bool isConfigMode() const { return _isConfigMode; }

    static constexpr int GridCols = 64;
    static constexpr int GridRows = 36;

  private:
    void updateLayout();
    static void drawGrid();
    void drawResizeHandles() const;
    void autoLinkPanels();
    [[nodiscard]] bool checkOverlap(const GridRect& rect, const std::shared_ptr<UIGamePanel>& ignorePanel) const;

    struct PanelData {
        std::shared_ptr<UIGamePanel> panel;
        GridRect originalGrid;
        GridRect grid;
    };

    void handleMousePressed(const raylib::rmath::Vector2& mousePos, float cellW, float cellH);
    void handleMouseReleased();
    void handleMouseDrag(const raylib::rmath::Vector2& mousePos, float cellW, float cellH);
    void updateResizeStack(PanelData& startPanel);
    void checkAndApplyResize(std::vector<PanelData>::iterator it, int newGridW, int newGridH, int deltaH);
    [[nodiscard]] bool isRectOverlappingStaticPanels(const GridRect& rect, int deltaH) const;
    [[nodiscard]] bool canPushStack(int deltaH) const;

    std::vector<PanelData> _panels;
    bool _isVisible{true};
    bool _isConfigMode{false};

    static constexpr float ResizeHandleSize = 15.0F;

    std::shared_ptr<UIGamePanel> _draggedPanel;
    std::shared_ptr<UIGamePanel> _resizedPanel;
    std::vector<std::reference_wrapper<PanelData>> _resizeStack;
    raylib::rmath::Vector2 _dragOffset{0.0F, 0.0F};
    GridRect _previewGrid{.x = 0, .y = 0, .w = 0, .h = 0};
};

}  // namespace zappy::gui::ui::components
