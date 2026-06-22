/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIGamePanel
*/

#include "UIGamePanel.hpp"

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "Color.hpp"
#include "graphics/AssetManager.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Rectangle.hpp"
#include "rmath/Vector2.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"
#include "rtext/Text.hpp"
#include "ui/IUIComponent.hpp"
#include "ui/components/UIPanel.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::components {

const raylib::Color UIGamePanel::PrimaryColor(255, 255, 255, 255);
const raylib::Color UIGamePanel::SecondaryColor(224, 224, 224, 255);
const raylib::Color UIGamePanel::TitleColor(0, 0, 0, 255);

UIGamePanel::UIGamePanel(float x, float y, float width, float height, const std::string& title)
    : _position(x, y), _size(width, height), _expandedHeight(height), _currentHeight(height) {
    _mainPanel = std::make_shared<UIPanel>(x, y, width, height, PrimaryColor);
    _contentPanel = std::make_shared<UIPanel>(x + Padding, y + DefaultHeaderHeight, width - (2 * Padding),
                                              height - DefaultHeaderHeight - Padding, SecondaryColor);

    auto font = graphics::AssetManager::getInstance().getFont(DefaultFontName);
    _titleText = std::make_shared<UIText>(title, font);
    _titleText->setColor(TitleColor);

    _mainPanel->addComponent(_contentPanel);
    _mainPanel->addComponent(_titleText);

    updateTextPosition();
}

UIGamePanel::UIGamePanel(float x, float y, float width, float height, const std::string& title,
                         const raylib::Color& primaryColor, const raylib::Color& secondaryColor,
                         const raylib::Color& titleColor)
    : _position(x, y), _size(width, height), _expandedHeight(height), _currentHeight(height) {
    _mainPanel = std::make_shared<UIPanel>(x, y, width, height, primaryColor);
    _contentPanel = std::make_shared<UIPanel>(x + Padding, y + DefaultHeaderHeight, width - (2 * Padding),
                                              height - DefaultHeaderHeight - Padding, secondaryColor);

    auto font = graphics::AssetManager::getInstance().getFont(DefaultFontName);
    _titleText = std::make_shared<UIText>(title, font);
    _titleText->setColor(titleColor);

    _mainPanel->addComponent(_contentPanel);
    _mainPanel->addComponent(_titleText);

    updateTextPosition();
}

void UIGamePanel::draw() {
    if (!_isVisible) {
        return;
    }

    _mainPanel->draw();

    raylib::rcore::Window::beginScissorMode(
        static_cast<int>(_position.x()), static_cast<int>(_position.y() + DefaultHeaderHeight),
        static_cast<int>(_size.x()), static_cast<int>(_currentHeight - DefaultHeaderHeight));

    if (!_isConfigMode) {
        for (auto& child : _contentChildren) {
            if (child->isVisible()) {
                child->draw();
            }
        }
    }

    raylib::rcore::Window::endScissorMode();
}

void UIGamePanel::update() {
    if (!_isVisible) {
        return;
    }

    float const targetHeight = (_isCollapsed && !_isConfigMode) ? DefaultHeaderHeight : _expandedHeight;
    float const lerpSpeed = LerpSpeedMultiplier * raylib::rcore::Window::frameTime();

    _currentHeight += (targetHeight - _currentHeight) * lerpSpeed;

    if (std::abs(_currentHeight - targetHeight) < 0.5F) {
        _currentHeight = targetHeight;
    }

    if (auto next = _nextPanel.lock()) {
        next->setPosition(_position.x(), _position.y() + _currentHeight + _nextPanelGap);
    }

    _mainPanel->setSize(_size.x(), _currentHeight);

    float contentHeight = _currentHeight - DefaultHeaderHeight - Padding;
    contentHeight = std::max(contentHeight, 0.0F);
    _contentPanel->setSize(_size.x() - (2.0F * Padding), contentHeight);

    _mainPanel->update();

    if (_currentHeight > DefaultHeaderHeight + 1.0F) {
        for (auto& child : _contentChildren) {
            if (child->isVisible()) {
                child->update();
            }
        }
    }
}

void UIGamePanel::handleEvent(const raylib::rcore::Event& event) {
    if (!_isVisible) {
        return;
    }

    raylib::rmath::Vector2 const mousePos = raylib::rcore::Event::getMousePositionStatic();
    raylib::rmath::Rectangle const headerRec{
        .x = _position.x(), .y = _position.y(), .width = _size.x(), .height = DefaultHeaderHeight};

    bool const isHovered = (mousePos.x() >= headerRec.x && mousePos.x() <= headerRec.x + headerRec.width &&
                            mousePos.y() >= headerRec.y && mousePos.y() <= headerRec.y + headerRec.height);

    if (isHovered && raylib::rcore::Event::isMouseButtonReleased(MouseLeftButton)) {
        _isCollapsed = !_isCollapsed;
    }

    _mainPanel->handleEvent(event);

    if (_isCollapsed) {
        return;
    }

    if (!_isConfigMode) {
        raylib::rmath::Vector2 const mousePos = raylib::rcore::Event::getMousePositionStatic();
        raylib::rmath::Rectangle const contentRec{.x = _position.x(),
                                                  .y = _position.y() + DefaultHeaderHeight,
                                                  .width = _size.x(),
                                                  .height = _currentHeight - DefaultHeaderHeight};
        if (raylib::rshapes::Shapes::checkCollisionPointRec(mousePos, contentRec)) {
            float const wheelMove = raylib::rcore::Event::getMouseWheelMoveStatic();
            if (wheelMove != 0.0F) {
                _scrollOffset -= wheelMove * ScrollSpeed;
                _scrollOffset = std::clamp(_scrollOffset, 0.0F, _maxScroll);
                updateChildrenLayout();
            }

            for (auto& child : _contentChildren) {
                if (child->isVisible()) {
                    child->handleEvent(event);
                }
            }
        }
    }
}

void UIGamePanel::setPosition(float x, float y) {
    _position.setX(x);
    _position.setY(y);
    _mainPanel->setPosition(x, y);
    _contentPanel->setPosition(x + Padding, y + DefaultHeaderHeight);

    updateTextPosition();
    updateChildrenLayout();
}

void UIGamePanel::setSize(float width, float height) {
    _size = raylib::rmath::Vector2(width, height);
    _expandedHeight = height;
    if (_isConfigMode) {
        _currentHeight = height;
    }
    _mainPanel->setSize(width, _currentHeight);
    float contentHeight = _currentHeight - DefaultHeaderHeight - Padding;
    contentHeight = std::max(contentHeight, 0.0F);
    _contentPanel->setSize(width - (2.0F * Padding), contentHeight);

    updateTextPosition();
    updateChildrenLayout();
}

bool UIGamePanel::isVisible() const { return _isVisible; }

void UIGamePanel::setVisible(bool visible) { _isVisible = visible; }

void UIGamePanel::addComponent(const std::shared_ptr<IUIComponent>& component) {
    if (component) {
        _contentChildren.emplace_back(component);
        updateChildrenLayout();
    }
}

void UIGamePanel::addHeaderComponent(const std::shared_ptr<IUIComponent>& component) {
    if (component && _mainPanel) {
        _mainPanel->addComponent(component);
    }
}

void UIGamePanel::removeComponent(const std::shared_ptr<IUIComponent>& component) {
    auto it = std::ranges::find(_contentChildren, component);
    if (it != _contentChildren.end()) {
        _contentChildren.erase(it);
        updateChildrenLayout();
    }
}

void UIGamePanel::setNextPanel(const std::shared_ptr<UIGamePanel>& panel, float gapPixels) {
    _nextPanel = panel;
    _nextPanelGap = gapPixels;
}

void UIGamePanel::updateTextPosition() {
    if (!_titleText) {
        return;
    }

    auto font = graphics::AssetManager::getInstance().getFont(DefaultFontName);
    int const fontSize = font && font->valid() ? font->baseSize() : DefaultFontSize;
    raylib::rtext::Font const defaultFont;
    float const textWidth =
        raylib::rtext::Text::measureText(font && font->valid() ? *font : defaultFont, _titleText->text(),
                                         static_cast<float>(fontSize), TextSpacing)
            .x();
    auto const textHeight = static_cast<float>(fontSize);

    _titleText->setPosition(_position.x() + ((_size.x() - textWidth) / 2.0F),
                            _position.y() + ((DefaultHeaderHeight - textHeight) / 2.0F));
}

void UIGamePanel::updateChildrenLayout() {
    if (_contentChildren.empty() || _customLayout) {
        _maxScroll = 0.0F;
        _scrollOffset = 0.0F;
        return;
    }

    float const innerPadding = Padding * 2.0F;
    float const paddingWidth = Padding * 4.0F;
    float const startX = _position.x() + paddingWidth;
    float const startY = _position.y() + DefaultHeaderHeight + innerPadding;

    float const availableWidth = _size.x() - (2.0F * paddingWidth);
    float const availableHeight = _expandedHeight - DefaultHeaderHeight - (2.0F * innerPadding);

    if (availableWidth <= 0.0F) {
        return;
    }

    float const itemHeight = 40.0F;
    float const spacing = 10.0F;
    float const totalContentHeight = ((itemHeight + spacing) * static_cast<float>(_contentChildren.size())) - spacing;
    _maxScroll = std::max(0.0F, totalContentHeight - availableHeight);
    _scrollOffset = std::clamp(_scrollOffset, 0.0F, _maxScroll);

    float currentY = startY - _scrollOffset;
    for (auto& child : _contentChildren) {
        child->setPosition(startX, currentY);
        child->setSize(availableWidth, itemHeight);
        bool const isInside = (currentY + itemHeight > startY) && (currentY < startY + availableHeight);
        child->setVisible(isInside);

        currentY += itemHeight + spacing;
    }
}

}  // namespace zappy::gui::ui::components
