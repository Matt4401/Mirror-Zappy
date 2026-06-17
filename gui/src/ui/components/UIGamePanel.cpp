/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIGamePanel
*/

#include "UIGamePanel.hpp"

#include <raylib.h>

#include <algorithm>
#include <cmath>
#include <memory>
#include <ranges>
#include <string>

#include "../../graphics/AssetManager.hpp"
#include "Color.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Vector2.hpp"
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

    auto font = graphics::AssetManager::getInstance().getFont("Minecraft");
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

    auto font = graphics::AssetManager::getInstance().getFont("Minecraft");
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

    BeginScissorMode(static_cast<int>(_position.x()), static_cast<int>(_position.y() + DefaultHeaderHeight),
                     static_cast<int>(_size.x()), static_cast<int>(_currentHeight - DefaultHeaderHeight));

    for (auto& child : _contentChildren) {
        if (child->isVisible()) {
            child->draw();
        }
    }

    EndScissorMode();
}

void UIGamePanel::update() {
    if (!_isVisible) {
        return;
    }

    float const targetHeight = _isCollapsed ? DefaultHeaderHeight : _expandedHeight;
    float const lerpSpeed = LerpSpeedMultiplier * raylib::rcore::Window::frameTime();
    float const previousHeight = _currentHeight;

    _currentHeight += (targetHeight - _currentHeight) * lerpSpeed;

    if (std::abs(_currentHeight - targetHeight) < 0.5F) {
        _currentHeight = targetHeight;
    }

    float const deltaHeight = _currentHeight - previousHeight;
    if (deltaHeight != 0.0F) {
        if (auto next = _nextPanel.lock()) {
            raylib::rmath::Vector2 const nextPos = next->getPosition();
            next->setPosition(nextPos.x(), nextPos.y() + deltaHeight);
        }
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
    Rectangle const headerRec = {_position.x(), _position.y(), _size.x(), DefaultHeaderHeight};

    bool const isHovered = (mousePos.x() >= headerRec.x && mousePos.x() <= headerRec.x + headerRec.width &&
                            mousePos.y() >= headerRec.y && mousePos.y() <= headerRec.y + headerRec.height);

    if (isHovered && raylib::rcore::Event::isMouseButtonReleased(MouseLeftButton)) {
        _isCollapsed = !_isCollapsed;
    }

    _mainPanel->handleEvent(event);

    if (_currentHeight > DefaultHeaderHeight + 1.0F) {
        for (auto& it : std::ranges::reverse_view(_contentChildren)) {
            if (it->isVisible()) {
                it->handleEvent(event);
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
}

void UIGamePanel::setSize(float width, float height) {
    _size = raylib::rmath::Vector2(width, height);
    _expandedHeight = height;
    if (!_isCollapsed) {
        _currentHeight = height;
    }
    _mainPanel->setSize(width, _currentHeight);
    float contentHeight = _currentHeight - DefaultHeaderHeight - Padding;
    contentHeight = std::max(contentHeight, 0.0F);
    _contentPanel->setSize(width - (2.0F * Padding), contentHeight);

    updateTextPosition();
}

bool UIGamePanel::isVisible() const { return _isVisible; }

void UIGamePanel::setVisible(bool visible) { _isVisible = visible; }

void UIGamePanel::addComponent(const std::shared_ptr<IUIComponent>& component) {
    if (component) {
        _contentChildren.emplace_back(component);
    }
}

void UIGamePanel::removeComponent(const std::shared_ptr<IUIComponent>& component) {
    auto it = std::ranges::find(_contentChildren, component);
    if (it != _contentChildren.end()) {
        _contentChildren.erase(it);
    }
}

void UIGamePanel::setNextPanel(const std::shared_ptr<UIGamePanel>& panel) { _nextPanel = panel; }

void UIGamePanel::updateTextPosition() {
    if (!_titleText) {
        return;
    }

    auto font = graphics::AssetManager::getInstance().getFont("Minecraft");
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

}  // namespace zappy::gui::ui::components
