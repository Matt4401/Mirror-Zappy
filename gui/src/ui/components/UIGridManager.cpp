/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIGridManager
*/

#include "UIGridManager.hpp"

#include <raylib.h>

#include <algorithm>
#include <cmath>
#include <functional>
#include <memory>
#include <optional>
#include <ranges>
#include <vector>

#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Vector2.hpp"
#include "ui/components/UIGamePanel.hpp"

namespace zappy::gui::ui::components {

UIGridManager::UIGridManager() = default;

void UIGridManager::draw() {
    if (!_isVisible) {
        return;
    }

    if (_isConfigMode) {
        drawGrid();
    }

    for (auto& data : _panels) {
        data.panel->draw();
    }

    if (_isConfigMode) {
        drawResizeHandles();
        if (_draggedPanel || _resizedPanel) {
            float const cellW = static_cast<float>(raylib::rcore::Window::screenWidth()) / static_cast<float>(GridCols);
            float const cellH =
                static_cast<float>(raylib::rcore::Window::screenHeight()) / static_cast<float>(GridRows);
            DrawRectangleLines(static_cast<int>(static_cast<float>(_previewGrid.x) * cellW),
                               static_cast<int>(static_cast<float>(_previewGrid.y) * cellH),
                               static_cast<int>(static_cast<float>(_previewGrid.w) * cellW),
                               static_cast<int>(static_cast<float>(_previewGrid.h) * cellH), GREEN);
        }
    }
}

void UIGridManager::update() {
    if (!_isVisible) {
        return;
    }

    float const cellW = static_cast<float>(raylib::rcore::Window::screenWidth()) / static_cast<float>(GridCols);
    float const cellH = static_cast<float>(raylib::rcore::Window::screenHeight()) / static_cast<float>(GridRows);

    autoLinkPanels();

    for (auto& data : _panels) {
        if (data.panel != _draggedPanel && data.panel != _resizedPanel) {
            data.panel->setSize(static_cast<float>(data.grid.w) * cellW, static_cast<float>(data.grid.h) * cellH);
            bool isRoot = true;
            for (const auto& other : _panels) {
                if (other.panel != data.panel) {
                    int const gap = data.grid.y - (other.grid.y + other.grid.h);
                    if (other.grid.x == data.grid.x && (gap == 0 || gap == 1)) {
                        isRoot = false;
                        break;
                    }
                }
            }
            if (isRoot) {
                data.panel->setPosition(static_cast<float>(data.grid.x) * cellW,
                                        static_cast<float>(data.grid.y) * cellH);
            }
        }
        data.panel->update();
    }
}

void UIGridManager::handleEvent(const raylib::rcore::Event& event) {
    if (!_isVisible) {
        return;
    }

    if (!_isConfigMode) {
        for (auto& _panel : std::ranges::reverse_view(_panels)) {
            _panel.panel->handleEvent(event);
        }
        return;
    }

    raylib::rmath::Vector2 const mousePos = raylib::rcore::Event::getMousePositionStatic();
    float const cellW = static_cast<float>(raylib::rcore::Window::screenWidth()) / static_cast<float>(GridCols);
    float const cellH = static_cast<float>(raylib::rcore::Window::screenHeight()) / static_cast<float>(GridRows);

    if (raylib::rcore::Event::isMouseButtonPressed(0)) {
        handleMousePressed(mousePos, cellW, cellH);
    } else if (raylib::rcore::Event::isMouseButtonReleased(0)) {
        handleMouseReleased();
    }

    handleMouseDrag(mousePos, cellW, cellH);
}

void UIGridManager::updateResizeStack(PanelData& startPanel) {
    _resizeStack.clear();
    std::reference_wrapper<PanelData> current = startPanel;
    while (true) {
        std::optional<std::reference_wrapper<PanelData>> next;
        for (auto& data : _panels) {
            if (&data == &current.get()) {
                continue;
            }
            int const gap = data.grid.y - (current.get().grid.y + current.get().grid.h);
            if (data.grid.x == current.get().grid.x && (gap == 0 || gap == 1)) {
                next.emplace(data);
                break;
            }
        }
        if (next.has_value()) {
            _resizeStack.emplace_back(next->get());
            current = next->get();
        } else {
            break;
        }
    }
}

void UIGridManager::handleMousePressed(const raylib::rmath::Vector2& mousePos, float cellW, float cellH) {
    for (auto& _panel : std::ranges::reverse_view(_panels)) {
        raylib::rmath::Vector2 const pos = _panel.panel->getPosition();
        float const w = static_cast<float>(_panel.grid.w) * cellW;
        float const h = static_cast<float>(_panel.grid.h) * cellH;
        Rectangle const handleRec = {.x = pos.x() + w - ResizeHandleSize,
                                     .y = pos.y() + h - ResizeHandleSize,
                                     .width = ResizeHandleSize,
                                     .height = ResizeHandleSize};

        if (CheckCollisionPointRec(mousePos.vector(), handleRec)) {
            _resizedPanel = _panel.panel;
            for (auto& data : _panels) {
                data.originalGrid = data.grid;
            }
            _previewGrid = _panel.grid;
            updateResizeStack(_panel);
            return;
        }
    }

    for (auto& it : std::ranges::reverse_view(_panels)) {
        raylib::rmath::Vector2 const pos = it.panel->getPosition();
        float const w = static_cast<float>(it.grid.w) * cellW;
        float const h = static_cast<float>(it.grid.h) * cellH;
        Rectangle const panelRec{.x = pos.x(), .y = pos.y(), .width = w, .height = h};

        if (CheckCollisionPointRec(mousePos.vector(), panelRec)) {
            _draggedPanel = it.panel;
            it.originalGrid = it.grid;
            _previewGrid = it.grid;
            _dragOffset = raylib::rmath::Vector2(mousePos.x() - pos.x(), mousePos.y() - pos.y());
            return;
        }
    }
}

void UIGridManager::handleMouseReleased() {
    if (_draggedPanel) {
        auto it = std::ranges::find_if(_panels, [this](const PanelData& d) { return d.panel == _draggedPanel; });
        if (it != _panels.end()) {
            if (!checkOverlap(_previewGrid, _draggedPanel)) {
                it->grid = _previewGrid;
            } else {
                it->grid = it->originalGrid;
            }
        }
        _draggedPanel.reset();
    }
    if (_resizedPanel) {
        auto it = std::ranges::find_if(_panels, [this](const PanelData& d) { return d.panel == _resizedPanel; });
        if (it != _panels.end()) {
            int const deltaH = it->grid.h - it->originalGrid.h;
            if (deltaH <= 0) {
                for (auto& spRef : _resizeStack) {
                    PanelData& sp = spRef.get();
                    sp.grid.y = sp.originalGrid.y + deltaH;
                }
            }
        }
        _resizedPanel.reset();
        _resizeStack.clear();
    }
}

bool UIGridManager::isRectOverlappingStaticPanels(const GridRect& rect, int deltaH) const {
    return std::ranges::any_of(_panels, [this, &rect, deltaH](const PanelData& data) {
        if (data.panel == _resizedPanel) {
            return false;
        }
        if (deltaH > 0) {
            bool const inStack = std::ranges::any_of(
                _resizeStack,
                [&data](const std::reference_wrapper<PanelData>& sp) { return sp.get().panel == data.panel; });
            if (inStack) {
                return false;
            }
        }
        bool const overlapX = (rect.x < data.grid.x + data.grid.w) && (rect.x + rect.w > data.grid.x);
        bool const overlapY = (rect.y < data.grid.y + data.grid.h) && (rect.y + rect.h > data.grid.y);
        return overlapX && overlapY;
    });
}

bool UIGridManager::canPushStack(int deltaH) const {
    return std::ranges::none_of(_resizeStack, [this, deltaH](const std::reference_wrapper<PanelData>& spRef) {
        PanelData const& sp = spRef.get();
        GridRect spTest = sp.originalGrid;
        spTest.y += deltaH;
        if (spTest.y + spTest.h > GridRows) {
            return true;
        }
        return isRectOverlappingStaticPanels(spTest, deltaH);
    });
}

void UIGridManager::checkAndApplyResize(std::vector<PanelData>::iterator it, int newGridW, int newGridH, int deltaH) {
    GridRect testRect = it->originalGrid;
    testRect.w = newGridW;
    testRect.h = newGridH;

    bool valid = !isRectOverlappingStaticPanels(testRect, deltaH);

    if (valid && deltaH > 0) {
        valid = canPushStack(deltaH);
    }

    if (valid) {
        it->grid.w = newGridW;
        it->grid.h = newGridH;
        _previewGrid = it->grid;

        for (auto& spRef : _resizeStack) {
            PanelData& sp = spRef.get();
            sp.grid.y = sp.originalGrid.y + std::max(0, deltaH);
        }
    }
}

void UIGridManager::handleMouseDrag(const raylib::rmath::Vector2& mousePos, float cellW, float cellH) {
    if (_draggedPanel) {
        float const newX = mousePos.x() - _dragOffset.x();
        float const newY = mousePos.y() - _dragOffset.y();
        _draggedPanel->setPosition(newX, newY);
        int const newGridX = std::clamp(static_cast<int>(std::round(newX / cellW)), 0, GridCols - _previewGrid.w);
        int const newGridY = std::clamp(static_cast<int>(std::round(newY / cellH)), 0, GridRows - _previewGrid.h);
        _previewGrid.x = newGridX;
        _previewGrid.y = newGridY;
    } else if (_resizedPanel) {
        auto it = std::ranges::find_if(_panels, [this](const PanelData& d) { return d.panel == _resizedPanel; });
        if (it != _panels.end()) {
            raylib::rmath::Vector2 const pos = it->panel->getPosition();
            float const newW = mousePos.x() - pos.x() + (ResizeHandleSize / 2.0F);
            float const newH = mousePos.y() - pos.y() + (ResizeHandleSize / 2.0F);
            int const newGridW =
                std::clamp(static_cast<int>(std::round(newW / cellW)), 2, GridCols - it->originalGrid.x);
            int const newGridH =
                std::clamp(static_cast<int>(std::round(newH / cellH)), 2, GridRows - it->originalGrid.y);
            int const deltaH = newGridH - it->originalGrid.h;
            checkAndApplyResize(it, newGridW, newGridH, deltaH);
        }
    }
}

void UIGridManager::addPanel(const std::shared_ptr<UIGamePanel>& panel, int gridX, int gridY, int gridW, int gridH) {
    if (!panel || gridX < 0 || gridY < 0 || gridW <= 0 || gridH <= 0 || gridX + gridW > GridCols ||
        gridY + gridH > GridRows) {
        return;
    }
    GridRect const newRect{.x = gridX, .y = gridY, .w = gridW, .h = gridH};
    if (checkOverlap(newRect, nullptr)) {
        return;
    }
    _panels.push_back(PanelData{.panel = panel,
                                .originalGrid = {.x = gridX, .y = gridY, .w = gridW, .h = gridH},
                                .grid = {.x = gridX, .y = gridY, .w = gridW, .h = gridH}});
}

void UIGridManager::setConfigMode(bool configMode) {
    _isConfigMode = configMode;
    for (auto& data : _panels) {
        data.panel->setConfigMode(configMode);
    }
}

void UIGridManager::drawGrid() {
    float const cellW = static_cast<float>(raylib::rcore::Window::screenWidth()) / static_cast<float>(GridCols);
    float const cellH = static_cast<float>(raylib::rcore::Window::screenHeight()) / static_cast<float>(GridRows);

    for (int i = 0; i <= GridCols; ++i) {
        DrawLine(static_cast<int>(static_cast<float>(i) * cellW), 0, static_cast<int>(static_cast<float>(i) * cellW),
                 raylib::rcore::Window::screenHeight(), ColorAlpha(LIGHTGRAY, 0.5F));
    }
    for (int i = 0; i <= GridRows; ++i) {
        DrawLine(0, static_cast<int>(static_cast<float>(i) * cellH), raylib::rcore::Window::screenWidth(),
                 static_cast<int>(static_cast<float>(i) * cellH), ColorAlpha(LIGHTGRAY, 0.5F));
    }
}

void UIGridManager::drawResizeHandles() const {
    float const cellW = static_cast<float>(raylib::rcore::Window::screenWidth()) / static_cast<float>(GridCols);
    float const cellH = static_cast<float>(raylib::rcore::Window::screenHeight()) / static_cast<float>(GridRows);

    for (const auto& data : _panels) {
        raylib::rmath::Vector2 const pos = data.panel->getPosition();
        float const w = static_cast<float>(data.grid.w) * cellW;
        float const h = static_cast<float>(data.grid.h) * cellH;
        Rectangle const handleRec{.x = pos.x() + w - ResizeHandleSize,
                                  .y = pos.y() + h - ResizeHandleSize,
                                  .width = ResizeHandleSize,
                                  .height = ResizeHandleSize};
        DrawRectangleRec(handleRec, ColorAlpha(RED, 0.8F));
    }
}

bool UIGridManager::checkOverlap(const GridRect& rect, const std::shared_ptr<UIGamePanel>& ignorePanel) const {
    return std::ranges::any_of(_panels, [&rect, &ignorePanel](const PanelData& data) {
        if (data.panel == ignorePanel) {
            return false;
        }
        bool const overlapX = (rect.x < data.grid.x + data.grid.w) && (rect.x + rect.w > data.grid.x);
        bool const overlapY = (rect.y < data.grid.y + data.grid.h) && (rect.y + rect.h > data.grid.y);
        return overlapX && overlapY;
    });
}

void UIGridManager::autoLinkPanels() {
    for (auto& data : _panels) {
        data.panel->setNextPanel(nullptr);
    }

    for (auto& topData : _panels) {
        for (auto& bottomData : _panels) {
            if (topData.panel == bottomData.panel) {
                continue;
            }
            if (topData.panel == _draggedPanel || bottomData.panel == _draggedPanel) {
                continue;
            }
            int const gap = bottomData.grid.y - (topData.grid.y + topData.grid.h);
            if (topData.grid.x == bottomData.grid.x && (gap == 0 || gap == 1)) {
                float const cellH = static_cast<float>(raylib::rcore::Window::screenHeight()) / GridRows;
                topData.panel->setNextPanel(bottomData.panel, static_cast<float>(gap) * cellH);
            }
        }
    }
}

void UIGridManager::setPosition(float /*x*/, float /*y*/) {}
void UIGridManager::setSize(float /*width*/, float /*height*/) {}
bool UIGridManager::isVisible() const { return _isVisible; }
void UIGridManager::setVisible(bool visible) { _isVisible = visible; }

}  // namespace zappy::gui::ui::components
